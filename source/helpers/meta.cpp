#include <iostream>
#include <windows.h>
#include <shobjidl.h> 
#include <SDL3/SDL.h>
#include "renderer.hpp"
#include <json/json.h>
#include <fstream>
using namespace std;

//Parse a json file
Json::Value ParseJSON(const char* path){
    Json::Value result;

    FILE* fp = fopen(path, "rb");
    if (!fp) {
        perror("fopen failed");
        return result;
    }

    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    rewind(fp);

    char* data = (char*)malloc(size + 1);
    fread(data, 1, size, fp);
    data[size] = 0;
    fclose(fp);

    Json::CharReaderBuilder builder;
    builder["allowTrailingCommas"] = true;
    std::string errs;
    std::istringstream stream(data);

    if (!Json::parseFromStream(builder, stream, &result, &errs)) {
        printf("JSON parse error: %s\n", errs.c_str());
    }

    free(data);
    return result;
}

//Brings up the file picker (to-do, clean up and shrink i just stole this from a microsoft example lol)
const char* GetFileUI(COMDLG_FILTERSPEC rgSpec[], UINT filterCount){
    static char filePath[MAX_PATH];

    HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | 
        COINIT_DISABLE_OLE1DDE);
    if (SUCCEEDED(hr))
    {
        IFileOpenDialog *pFileOpen;

        // Create the FileOpenDialog object.
        hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, 
                IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));

        if (SUCCEEDED(hr))
        {
            pFileOpen->SetFileTypes(filterCount, rgSpec);
            pFileOpen->SetFileTypeIndex(1);

            // Show the Open dialog box.
            hr = pFileOpen->Show(NULL);

            // Get the file name from the dialog box.
            if (SUCCEEDED(hr))
            {
                IShellItem *pItem;
                hr = pFileOpen->GetResult(&pItem);
                if (SUCCEEDED(hr))
                {
                    PWSTR pszFilePath;
                    hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

                    // Display the file name to the user.
                    if (SUCCEEDED(hr))
                    {
                        WideCharToMultiByte(CP_ACP, 0, pszFilePath, -1, filePath, MAX_PATH, NULL, NULL);
                        CoTaskMemFree(pszFilePath);
                        return filePath;
                    }
                    pItem->Release();
                }
            }
            pFileOpen->Release();
        }
        CoUninitialize();
    }

    return "";
}

//Show a error message
void ShowError(const char* Message){
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "ERROR", Message, window);
}

void File_WriteEnd(const char* FilePath, const char* Message){
    std::ofstream out;
    out.open(FilePath, std::ios::app);
    std::string str = Message;
    out << str;
    out.close();
}