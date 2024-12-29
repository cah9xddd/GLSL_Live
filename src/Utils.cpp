#include "Utils.h"

std::string ReadTextFromFile(std::string_view file_path_) noexcept
{
    std::string text;

    if (file_path_.empty())
    {
        LOG_ERROR("File path is empty");
        return text;
    }

    // Creating an input file stream
    std::ifstream in_file(file_path_.data(), std::ios::in | std::ios::binary);

    // Check if the file is opened
    if (!in_file)
    {
        LOG_ERROR("Failed to open file: {}", file_path_);
        return text;
    }


    // Read file content directly into the text string
    text.assign((std::istreambuf_iterator<char>(in_file)), std::istreambuf_iterator<char>());

    // Check if the reading was successful
    if (!in_file.good() && !in_file.eof())
    {
        LOG_ERROR("Error reading the file: {}", file_path_);
        return "";  // Return empty string in case of an error
    }

    return text;
}

std::string CreateTemporaryCopyOfFile(std::string_view file_path_) noexcept
{
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
}

bool WriteTextToFile(std::string_view file_path_, std::string_view text_) noexcept
{

    if (file_path_.empty())
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
    auto temporary_copy = CreateTemporaryCopyOfFile(file_path_);

    std::filesystem::path dir = std::filesystem::path(file_path_).parent_path();

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
    std::ofstream out_file(file_path_.data(), std::ios::out | std::ios::binary | std::ios::trunc);

    // Check if the file is opened
    if (!out_file)
    {
        LOG_ERROR("Failed to open file: {}", file_path_);
        return false;
    }

    // Write the text to the file
    out_file << text_;

    // Check if the writing was successful
    if (!out_file.good())
    {
        LOG_ERROR("Error writing to the file: {}", file_path_);
        // Returning temporary copy of the file if we have one
        if (!temporary_copy.empty())
        {
            try  // Trying to restore the temporary copy of the file
            {
                std::filesystem::copy(temporary_copy,
                                      file_path_,
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
    std::string path = std::filesystem::current_path().string();
    std::replace(path.begin(), path.end(), '\\', '/');
    return path;
}