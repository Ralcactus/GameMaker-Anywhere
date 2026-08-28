#include <iostream>
#include <windows.h>
#include <shobjidl.h> 
#include <SDL3/SDL.h>
#include "renderer.hpp"

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

void ShowError(const char* Message){
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "ERROR", Message, window);
}
