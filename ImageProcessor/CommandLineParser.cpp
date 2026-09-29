/**
 * @file CommandLineParser.cpp
 */

#include "CommandLineParser.h"
#include "Exceptions.h"
#include "NumberParser.h"

#include <iostream>
#include <string>
#include <vector>

namespace ip {

namespace {
    /// argv 의 다음 인자를 안전하게 가져온다.
    std::string nextArg(int argc, char* argv[], int& i, const std::string& flag) {
        if (i + 1 >= argc) {
            throw ArgumentError(flag + ": missing value");
        }
        return argv[++i];
    }

    std::vector<std::string> splitAndTrim(const std::string& raw, char delimiter)
    {
        std::vector<std::string> tokens;
        std::size_t start = 0;

        while (true) {
            const std::size_t end = raw.find(delimiter, start);
            const std::string piece = raw.substr(start, end == std::string::npos ? std::string::npos : end - start);

            const std::size_t first = piece.find_first_not_of(" \t");
            const std::size_t last = piece.find_last_not_of(" \t");
            const std::string trimmed = (first == std::string::npos) ? "" : piece.substr(first, last - first + 1);

            if (trimmed.empty()) {
                throw ArgumentError("--pipeline contains an empty filter token");
            }
            tokens.push_back(trimmed);

            if (end == std::string::npos) {
                break;
            }
            start = end + 1;
        }

        return tokens;
    }

} // anonymous namespace

std::string peekLogPath(int argc, char* argv[]) {
    for (int i = 1; i + 1 < argc; ++i) {
        const std::string arg = argv[i];
        if (arg == "--log" || arg == "-l") {
            return argv[i + 1];
        }
    }
    return "";
}

ProgramOptions CommandLineParser::parse(int argc, char* argv[]) {
    ProgramOptions options;

    std::string filterArg;
    std::string pipelineArg;
    std::string thresholdArg;
    bool hasFilter = false;
    bool hasPipeline = false;
    bool hasThreshold = false;

    for (int i = 1; i < argc; ++i) {
        const std::string arg = argv[i];

        if (arg == "--input" || arg == "-i") {
            options.inputPath = nextArg(argc, argv, i, arg);
        }
        else if (arg == "--output" || arg == "-o") {
            options.outputPath = nextArg(argc, argv, i, arg);
        }
        else if (arg == "--filter" || arg == "-f") {
            filterArg = nextArg(argc, argv, i, arg);
            hasFilter = true;
        }
        else if (arg == "--threshold" || arg == "-t") {
            thresholdArg = nextArg(argc, argv, i, arg);
            hasThreshold = true;
        }
        else if (arg == "--pipeline" || arg == "-p") {
            pipelineArg = nextArg(argc, argv, i, arg);
            hasPipeline = true;
        }
        else if (arg == "--threads") {
            const std::string threadsArg = nextArg(argc, argv, i, arg);
            int threads = 0;
            std::string parseError;
            if (!NumberParser::tryParseInt(threadsArg, threads, parseError)) {
                throw ArgumentError("--threads: '" + threadsArg + "' is not a valid integer (" + parseError + ")");
            }
            if (threads < 1) {
                throw ArgumentError("--threads must be at least 1 (got " + std::to_string(threads) + ")");
            }
            options.threadCount = static_cast<unsigned>(threads);
        }
        else if (arg == "--log" || arg == "-l") {
            options.logPath = nextArg(argc, argv, i, arg);
        }
        else if (arg == "--help" || arg == "-h") {
            printUsage(argv[0]);
            std::exit(0);
        }
        else {
            throw ArgumentError("Unknown option: " + arg);
        }
    }

    // 필수 인자 검증
    if (options.inputPath.empty()) {
        throw ArgumentError("--input is required");
    }
    if (options.outputPath.empty()) {
        throw ArgumentError("--output is required");
    }
    if (!hasFilter && !hasPipeline) {
        throw ArgumentError("--filter or --pipeline is required");
    }
    if (hasFilter && hasPipeline) {
        throw ArgumentError("--filter and --pipeline cannot be used together");
    }
    if (hasThreshold && hasPipeline) {
        throw ArgumentError("--threshold and --pipeline cannot be used together");
    }

    if (hasPipeline) {
        options.filters = splitAndTrim(pipelineArg, ',');
    }
    else if (hasThreshold) {
        const std::string filterName = filterArg.substr(0, filterArg.find(':'));
        if (filterName == "threshold") {
            throw ArgumentError("--threshold cannot be combined with a --filter threshold token (duplicate threshold value)");
        }

        int threshold = 0;
        std::string parseError;
        if (!NumberParser::tryParseInt(thresholdArg, threshold, parseError)) {
            throw ArgumentError("--threshold: '" + thresholdArg + "' is not a valid integer (" + parseError + ")");
        }
        if (threshold < 0 || threshold > 255) {
            throw ArgumentError("--threshold must be between 0 and 255 (got " + std::to_string(threshold) + ")");
        }

        options.filters.push_back(filterArg);
        options.filters.push_back("threshold:" + std::to_string(threshold));
    }
    else {
        options.filters.push_back(filterArg);
    }

    return options;
}

void CommandLineParser::printUsage(const std::string& exeName) {
    std::cout
        << "Usage:\n"
        << "  " << exeName << " --input <path> --output <path> --filter <token> [--threshold <0-255>]\n"
        << "  " << exeName << " --input <path> --output <path> --pipeline \"<token>,<token>,...\"\n\n"
        << "Options:\n"
        << "  -i, --input     <path>    Input BMP file (24-bit, uncompressed)\n"
        << "  -o, --output    <path>    Output BMP file\n"
        << "  -f, --filter    <token>   Single filter token (e.g. grayscale, threshold:128)\n"
        << "  -t, --threshold <0-255>   Threshold value; requires --filter, cannot combine with --pipeline\n"
        << "  -p, --pipeline  <tokens>  Comma-separated filter chain (e.g. \"grayscale,blur,threshold:128\")\n"
        << "      --threads   <N>       Thread count for row-parallel filters (default: hardware_concurrency())\n"
        << "  -l, --log       <path>    Append run log (timestamps, filter timing, errors) to this file\n"
        << "  -h, --help                Show this message\n\n"
        << "Filter tokens (name, or name:arg:arg with colon-separated arguments):\n"
        << "  grayscale, horizontal, vertical, blur, sharpen, hist   no arguments\n"
        << "  threshold:<0-255>                                     binarize at threshold\n"
        << "  brightness:<delta>                                    add delta to each channel\n"
        << "  contrast:<factor>                                     scale around 128\n"
        << "  crop:<x>:<y>:<w>:<h>                                  crop rectangle\n"
        << "  resize:<w>:<h>                                        resize to w x h\n\n"
        << "Examples:\n"
        << "  " << exeName << " --input input.bmp --output result.bmp --filter grayscale\n"
        << "  " << exeName << " --input input.bmp --output result.bmp --filter blur --threshold 128\n"
        << "  " << exeName << " --input input.bmp --output result.bmp --pipeline \"grayscale,blur,threshold:128\"\n";
}

} // namespace ip
