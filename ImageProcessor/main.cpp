/**
 * @file main.cpp
 * @brief ImageProcessor 진입점 — 지원자가 작성해야 할 파일입니다.
 *
 * BMP 입출력과 커맨드라인 파싱은 제공된 코드가 처리합니다.
 * 본 과제에서 작성해야 할 것은 단 하나입니다:
 *
 *     ▶ 이미지 처리 필터 2개 이상 구현 + main 의 TODO 위치에 연결
 *
 * 또한 일관된 컨벤션과 예외 처리, 메모리 안정성도 함께 평가됩니다.
 */

#include "BmpParser.h"
#include "CommandLineParser.h"
#include "ImageBuffer.h"
#include "Exceptions.h"
#include "FilterBase.h"
#include "FilterFactory.h"
#include "Logger.h"

#include <algorithm>
#include <chrono>
#include <iostream>
#include <memory>
#include <thread>
#include <utility>
#include <vector>

int main(int argc, char* argv[]) {
    const ip::Logger logger(ip::peekLogPath(argc, argv));

    try {
        // ── CLI 인자 파싱 (제공된 코드) ─────────────────────────
        const ip::ProgramOptions options = ip::CommandLineParser::parse(argc, argv);

        std::vector<std::unique_ptr<ip::FilterBase>> filters;
        filters.reserve(options.filters.size());
        for (const std::string& token : options.filters) {
            filters.push_back(ip::FilterFactory::create(token, options.threadCount));
        }

        // ── BMP 로드 (제공된 코드) ──────────────────────────────
        ip::ImageBuffer image = ip::BmpParser::loadFromFile(options.inputPath);
        std::cout << "Loaded: " << image.width() << " x " << image.height() << "\n";
        logger.info("Loaded " + options.inputPath + " (" +
            std::to_string(image.width()) + "x" + std::to_string(image.height()) + ")");

        const unsigned effectiveThreads = (options.threadCount != 0)
            ? options.threadCount
            : std::max(1u, std::thread::hardware_concurrency());
        logger.info("Thread count: " + std::to_string(effectiveThreads) +
            (options.threadCount == 0 ? " (auto)" : ""));

        for (const auto& filter : filters) {
            logger.info("Filter start: " + filter->name());
            const auto start = std::chrono::steady_clock::now();

            image = filter->apply(std::move(image));

            const auto end = std::chrono::steady_clock::now();
            const double elapsedMs = std::chrono::duration<double, std::milli>(end - start).count();
            logger.info("Filter end: " + filter->name() + " (" + std::to_string(elapsedMs) + " ms)");
        }

        // ── BMP 저장 (제공된 코드) ──────────────────────────────
        ip::BmpParser::saveToFile(options.outputPath, image);
        std::cout << "Saved:  " << options.outputPath << "\n";
        logger.info("Saved " + options.outputPath);
        return 0;
    }
    catch (const ip::ArgumentError& e) {
        std::cerr << e.what() << "\n\n";
        logger.error(e.what());
        ip::CommandLineParser::printUsage(argc > 0 ? argv[0] : "ImageProcessor");
        return 4;
    }
    catch (const ip::BmpParseError& e) {
        std::cerr << e.what() << std::endl;
        logger.error(e.what());
        return 2;
    }
    catch (const ip::FilterError& e) {
        std::cerr << e.what() << std::endl;
        logger.error(e.what());
        return 3;
    }
    catch (const std::exception& e) {
        std::cerr << "Unexpected error: " << e.what() << std::endl;
        logger.error(e.what());
        return 1;
    }
}
