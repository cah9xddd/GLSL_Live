#pragma once

#include "PCH.h"

struct FileUtils {
    static std::string_view application_path; /**< Path to the application. DO NOT USE IT. Use
                                                 GetApplicationPath() instead. */
};

/**
 * @brief Function which read text from a file
 *
 * @param file_path_ Path to the file
 *
 * @return Source code of the shader or empty string in case of an error
 */
std::string ReadTextFromFile(std::string_view file_path_) noexcept;

/**
 * @brief Function which creates temporary copy of the file
 *
 * @param file_path_ Path to the file
 *
 * @return file_path_ + ".tmp" extension or empty string in case of an error
 */

std::string CreateTemporaryCopyOfFile(std::string_view file_path_) noexcept;

/**
 * @brief Function which write text to a file
 *
 * @param file_name_ Name of the file
 * @param file_path_ Path to the file
 * @param text_ Text to write to the file
 *
 * @remark If the file already exists, it will be overwritten
 *
 * @return true if the file was written successfully, false otherwise
 */
bool WriteTextToFile(std::string_view file_path_, std::string_view text_) noexcept;

/**
 * @brief Function which loads text from a file
 *
 * @param file_path_ Path to the file
 *
 * @return Text content of the file or an empty string in case of an error
 */
std::string LoadTextFromFile(std::string_view file_path_) noexcept;

/**
 * @brief Function which returns the path to the application
 *
 * @return Path to the application
 */
[[nodiscard]] std::string GetApplicationPath();
