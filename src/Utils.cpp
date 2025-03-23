#include "Utils.h"

std::string_view FileUtils::application_path = {};

std::string ReadTextFromFile(std::string_view file_path_) noexcept
{
    std::string text;

    auto full_path = GetApplicationPath() + "/" + std::string(file_path_);

    if (full_path.empty())
    {
        LOG_ERROR("File path is empty");
        return text;
    }

    // Creating an input file stream
    std::ifstream in_file(full_path.data(), std::ios::in | std::ios::binary);

    // Check if the file is opened or exists
    if (!in_file)
    {
        LOG_ERROR("Failed to open file: {}", full_path);
        return text;
    }


    // Read file content directly into the text string
    text.assign((std::istreambuf_iterator<char>(in_file)), std::istreambuf_iterator<char>());

    // Check if the reading was successful
    if (!in_file.good() && !in_file.eof())
    {
        LOG_ERROR("Error reading the file: {}", full_path);
        return "";  // Return empty string in case of an error
    }

    return text;
}

std::string CreateTemporaryCopyOfFile(std::string_view file_path_) noexcept
{

    if (file_path_.empty())
    {
        LOG_ERROR("File path is empty");
        return "";
    }

    if (!std::filesystem::exists(file_path_)) { return ""; }

    std::string temp_file_path = std::string(file_path_) + ".tmp";

    try  // Trying to create a temporary copy of the file
    {
        std::filesystem::copy(file_path_,
                              temp_file_path,
                              std::filesystem::copy_options::overwrite_existing);
    }
    catch (const std::exception& e)
    {
        LOG_ERROR("Failed to create a temporary copy of the file: {}", e.what());
        return "";
    }

    return temp_file_path;
}

bool WriteTextToFile(std::string_view file_path_, std::string_view text_) noexcept
{

    auto full_path = GetApplicationPath() + "/" + std::string(file_path_);

    if (full_path.empty())
    {
        LOG_ERROR("File path is empty");
        return false;
    }

    if (text_.empty())
    {
        LOG_ERROR("Text is empty");
        return false;
    }

    // Creating a temporary copy of the file
    auto temporary_copy = CreateTemporaryCopyOfFile(full_path);

    std::filesystem::path dir = std::filesystem::path(full_path).parent_path();

    if (!std::filesystem::exists(dir))
    {
        try
        {
            std::filesystem::create_directories(dir);  // Create the directory
        }
        catch (const std::filesystem::filesystem_error& e)
        {
            LOG_ERROR("Failed to create directory: {}", e.what());
            return "";
        }
    }

    // Creating an output file stream
    std::ofstream out_file(full_path.data(), std::ios::out | std::ios::binary | std::ios::trunc);

    // Check if the file is opened
    if (!out_file)
    {
        LOG_ERROR("Failed to open file: {}", full_path);
        return false;
    }

    // Write the text to the file
    out_file << text_;

    // Check if the writing was successful
    if (!out_file.good())
    {
        LOG_ERROR("Error writing to the file: {}", full_path);
        // Returning temporary copy of the file if we have one
        if (!temporary_copy.empty())
        {
            try  // Trying to restore the temporary copy of the file
            {
                std::filesystem::copy(temporary_copy,
                                      full_path,
                                      std::filesystem::copy_options::overwrite_existing);
                std::filesystem::remove(temporary_copy);
            }
            catch (const std::exception& e)
            {
                LOG_ERROR("Failed to restore the temporary copy of the file: {}", e.what());
            }
        }
        return false;
    }

    // If we have a temporary copy, we remove it
    if (!temporary_copy.empty())
    {
        try  // Trying to remove the temporary copy of the file
        {
            std::filesystem::remove(temporary_copy);
        }
        catch (const std::exception& e)
        {
            LOG_ERROR("Failed to remove the temporary copy of the file: {}", e.what());
            return false;
        }
    }

    return true;
}

std::string GetApplicationPath()
{
    std::filesystem::path exe_path =
        std::filesystem::canonical(FileUtils::application_path).parent_path();
    std::string path = exe_path.string();
    std::replace(path.begin(), path.end(), '\\', '/');
    return path;
}

std::string LoadTextFromFile(std::string_view file_path_) noexcept
{
    auto full_path = GetApplicationPath() + "/" + std::string(file_path_);

    if (full_path.empty())
    {
        LOG_ERROR("File path is empty");
        return "";
    }

    if (!std::filesystem::exists(full_path))
    {
        LOG_ERROR("File does not exist: {}", full_path);
        return "";
    }

    // Creating an input file stream
    std::ifstream in_file(full_path.data(), std::ios::in | std::ios::binary);

    // Check if the file is opened
    if (!in_file)
    {
        LOG_ERROR("Failed to open file: {}", full_path);
        return "";
    }

    // Read file content directly into a string
    std::string text((std::istreambuf_iterator<char>(in_file)), std::istreambuf_iterator<char>());

    // Check if the reading was successful
    if (!in_file.good() && !in_file.eof())
    {
        LOG_ERROR("Error reading the file: {}", full_path);
        return "";
    }

    return text;
}