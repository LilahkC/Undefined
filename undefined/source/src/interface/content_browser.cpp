#include "interface/content_browser.h"

#include "Resources/resource_manager.h"
#include <Resources/texture.h>

#include "utils/utils.h"

void ContentBrowser::DisplayDirectories(const std::filesystem::path& path)
{
    std::string name = path.filename().string();
    mIsDirectory = std::filesystem::is_directory(path);

    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_FramePadding;

    if (mIsDirectory)
    {
        for (const auto& entry : std::filesystem::directory_iterator(path))
        {
            if (entry.is_directory())
            {
                flags |= ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
                break;
            }
            else
            {
                flags |= ImGuiTreeNodeFlags_Leaf;
                break;
            }
        }

        if (mCurrentPath == path)
        {
            flags |= ImGuiTreeNodeFlags_Selected;
        }

        if (path == mPath)
        {
            flags |= ImGuiTreeNodeFlags_DefaultOpen;
        }
    }

    if (mRenamingPath == path)
    {
        name = "##";
    }

    ImGui::Image(Utils::IntToPointer<ImTextureID>(ResourceManager::Get<Texture>("imgui/folder.png")->GetID()), ImVec2(13, 13));

    ImGui::SameLine();

    if (ImGui::TreeNodeEx(name.c_str(), flags))
    {
        if (ImGui::IsItemHovered() && !ImGui::IsItemToggledOpen() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
        {
            std::string file = "start explorer ";
            file += '"' + absolute(path).string() + '"';
            system(file.c_str());
        }

        if (mIsDirectory)
        {
            if (!ImGui::IsItemToggledOpen() && ImGui::IsItemClicked(ImGuiMouseButton_Left))
            {
                mCurrentPath = path;
            }

            //For every folder we call the function to display what's inside
            for (const auto& entry : std::filesystem::directory_iterator(path))
            {
                if (entry.is_directory())
                {
                    DisplayDirectories(entry);
                }
            }
        }
        ImGui::TreePop();
    }

    else
    {
        if (!ImGui::IsItemToggledOpen() && ImGui::IsItemClicked(ImGuiMouseButton_Left))
        {
            mCurrentPath = path;
        }
    }

    RightClickWindow(path);

    if (mRenamingPath == path)
    {
        ImGui::SameLine();
        RenameItem();
    }
}

void ContentBrowser::TextCentered(const std::string& text)
{
    auto windowWidth = ImGui::GetWindowSize().x;
    auto textWidth = ImGui::CalcTextSize(text.c_str()).x;

    ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
    ImGui::TextWrapped(text.c_str());
}

void ContentBrowser::SetImageValues(const std::filesystem::path& path, ImTextureID& imageID, ImVec2& imageSize)
{
    if (mIsDirectory)
    {
        std::shared_ptr<Texture> folder = ResourceManager::Get<Texture>("imgui/folder.png");

        imageSize = ImVec2((float)folder->GetWidth(), (float)folder->GetHeight());
        imageID = Utils::IntToPointer<ImTextureID>(folder->GetID());
    }
    else
    {
        std::string name = path.generic_string();
        std::string parentName = path.parent_path().filename().generic_string();
        size_t pos = name.find(parentName);
        std::string newName = name.substr(pos);

        //If it's a texture we put the image of the texture instead of a basic file image
        if (path.string().ends_with(".jpg") || path.string().ends_with(".png"))
        {
            imageSize = ImVec2(80.f, 80.f);
            if (ResourceManager::Contains(newName))
            {

                imageID = Utils::IntToPointer<ImTextureID>(ResourceManager::Get<Texture>(newName)->GetID());
            }
            else
            {
                std::shared_ptr<Texture> file = ResourceManager::Get<Texture>("imgui/file.png");
            }
        }

        else if (path.string().ends_with(".obj"))
        {
            imageSize = ImVec2(80.f, 80.f);
            imageID = Utils::IntToPointer<ImTextureID>(ResourceManager::Get<Texture>("imgui/obj_file.png")->GetID());
        }

        else if (path.string().ends_with(".cpp") || path.string().ends_with(".h") || path.string().ends_with(".hpp"))
        {
            imageSize = ImVec2(80.f, 80.f);
            imageID = Utils::IntToPointer<ImTextureID>(ResourceManager::Get<Texture>("imgui/visual_studio.png")->GetID());
        }

        else
        {
            std::shared_ptr<Texture> file = ResourceManager::Get<Texture>("imgui/file.png");
            imageSize = ImVec2((float)file->GetWidth(), (float)file->GetHeight());
            imageID = Utils::IntToPointer<ImTextureID>(file->GetID());
        }
    }
}

void ContentBrowser::DisplayText(const std::filesystem::path& filepath, const std::string& filename, ImVec2& imageSize)
{
    //If the text is larger than the image size it wrap on multiple lines else it's centered
    if (ImGui::CalcTextSize(filename.c_str()).x > imageSize.x)
    {
        if (mRenamingPath != filepath)
        {
            ImGui::TextWrapped(filename.c_str());
        }
        else
        {
            RenameItemMultiline();
        }
    }

    else
    {
        if (mRenamingPath != filepath)
        {
            TextCentered(filename);
        }
        else
        {
            RenameItem();
        }
    }
}

void ContentBrowser::InteractionWithItems(const std::filesystem::path& path, bool isBackFolder)
{
    bool isClicked = ImGui::IsMouseClicked(ImGuiMouseButton_Left);

    //If we hover on a window we add a background color 
    if (ImGui::IsWindowHovered())
    {
        int clickCount = ImGui::GetMouseClickedCount(ImGuiMouseButton_Left);

        mHoveredPath = path;
        mIsAnythingHovered = true;

        //If we click on a window we select it 
        if (isClicked)
        {
            mSelectedPath = path;
            mIsAnythingSelected = true;
        }

        //If a window is selected and we click somewhere else it deselect it
        if (clickCount && (clickCount % 2 == 0))
        {
            if (mIsDirectory)
            {
                if (!isBackFolder)
                {
                    mCurrentPath = path;
                }
                else
                {
                    mCurrentPath = mCurrentPath.parent_path();
                }
            }
            else
            {
                std::string file = "start explorer ";
                file += '"' + absolute(path).string() + '"';
                system(file.c_str());
            }
        }

    }

    //If a window is selected and we click somewhere else it deselect it
    if (mSelectedPath == path && !ImGui::IsWindowHovered() && isClicked)
    {
        mSelectedPath = "";
        mIsAnythingSelected = false;
        mRenamingPath = "";
    }
}

void ContentBrowser::GoBackFolder(const std::filesystem::path& path)
{
    mIsAnythingHovered = false;
    mIsDirectory = true;
    ImGui::SameLine();

    if (path.generic_string() + "/" != mPath.generic_string() && path != mPath)
    {
        ImTextureID imageID;

        if (mSelectedPath == mBackFolder)
        {
            ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.7f, 0.7f, 0.7f, 0.7f));
            mCanPop = true;
        }
        else if (mHoveredPath == mBackFolder)
        {
            ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.5f, 0.5f, 0.5f, 0.5f));
            mCanPop = true;
        }

        ImGui::BeginChild("BackFolder", ImVec2(100, 120), ImGuiChildFlags_AlwaysUseWindowPadding);
        imageID = Utils::IntToPointer<ImTextureID>(ResourceManager::Get<Texture>("imgui/folder.png")->GetID());
        ImGui::Image(imageID, ImVec2(80, 80));

        InteractionWithItems(mBackFolder, true);

        TextCentered("../");

        ImGui::EndChild();

        if (mCanPop)
        {
            ImGui::PopStyleColor();
            mCanPop = false;
        }
    }
}

void ContentBrowser::LoadFolders(const std::filesystem::path& path)
{
    // For loop that goes through every file/folder in a path and displays them
    for (std::filesystem::directory_entry entry : std::filesystem::directory_iterator(path))
    {
        if (entry.is_directory())
        {
            mCurrPathArray.push_back(entry);
        }
    }
}

void ContentBrowser::LoadFiles(const std::filesystem::path& path)
{
    for (std::filesystem::directory_entry entry : std::filesystem::directory_iterator(path))
    {
        if (!entry.is_directory())
        {
            mCurrPathArray.push_back(entry);
        }
    }
}

void ContentBrowser::RenameItem()
{
    ImGuiInputTextFlags flags = ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_EscapeClearsAll;
    std::string name = mRenamingPath.filename().string();
    std::string oldName = mRenamingPath.filename().string();
    std::filesystem::path path = mRenamingPath.parent_path();

    char* newName = (char*)name.c_str();

    ImGui::SetKeyboardFocusHere();
    ImGui::PushItemWidth(ImGui::CalcTextSize(name.c_str()).x + 5);
    if (ImGui::InputText("##label", (char*)name.c_str(), 256, flags))
    {
        if (std::filesystem::is_directory(mRenamingPath))
        {
            mCurrentPath = (char*)name.c_str();
        }

        std::filesystem::path newPath = path.generic_string();
        newPath += "/";
        newPath += (char*)name.c_str();

        if (std::filesystem::is_directory(mRenamingPath))
        {
            ResourceManager::RenameFolder(oldName, (char*)name.c_str());
            //Change current path to the new path so the back folder does not crash (since the current path does not exist it doesn't have a parent)
            std::string newCurrentPath = mRenamingPath.parent_path().generic_string() + "/";
            newCurrentPath += newName;
            mCurrentPath = newCurrentPath;
        }

        std::filesystem::rename(mRenamingPath, newPath);
        mRenamingPath = "";
    }
}

void ContentBrowser::RenameItemMultiline()
{
    ImGuiInputTextFlags flags = ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_EscapeClearsAll;
    std::string name = mRenamingPath.filename().string();
    std::string oldName = mRenamingPath.filename().string();
    std::filesystem::path path = mRenamingPath.parent_path();

    char* newName = (char*)name.c_str();

    ImGui::SetKeyboardFocusHere();

    //Change so it goes to another line when you are at the end and not expand it
    if (ImGui::InputTextMultiline("##label", (char*)name.c_str(), 256, ImVec2(50, 40), flags))
    {
        if (std::filesystem::is_directory(mRenamingPath))
        {
            mCurrentPath = (char*)name.c_str();
        }

        std::filesystem::path newPath = path.generic_string();
        newPath += "/";
        newPath += (char*)name.c_str();

        if (std::filesystem::is_directory(mRenamingPath))
        {
            ResourceManager::RenameFolder(oldName, (char*)name.c_str());
            //Change current path to the new path so the back folder does not crash (since the current path does not exist it doesn't have a parent)
            std::string newCurrentPath = mRenamingPath.parent_path().generic_string() + "/";
            newCurrentPath += newName;
            mCurrentPath = newCurrentPath;
        }

        std::filesystem::rename(mRenamingPath, newPath);
        mRenamingPath = "";
    }
}

void ContentBrowser::RightClickWindow(const std::filesystem::path& path)
{
    if (ImGui::BeginPopupContextItem(path.string().c_str()))
    {
        if (ImGui::Button("Open in explorer"))
        {
            std::string explorer = "start explorer /select,";
            explorer += absolute(path).string();
            system(explorer.c_str());
            ImGui::CloseCurrentPopup();
        }

        if (ImGui::Button("Rename"))
        {
            mRenamingPath = path;
            mSelectedPath = mRenamingPath;
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }
}

void ContentBrowser::DisplayActualDirectory(const std::filesystem::path& currentPath)
{
    mIsAnythingHovered = false;

    GoBackFolder(currentPath);
    ImGui::SameLine();

    LoadFolders(currentPath);
    LoadFiles(currentPath);

    // For loop that goes through every file/folder in a path and displays them
    for (int i = 0; i < mCurrPathArray.size(); i++)
    {
        mIsDirectory = mCurrPathArray[i].is_directory();

        std::string filename = mCurrPathArray[i].path().filename().string();
        std::filesystem::path filepath = mCurrPathArray[i].path();

        ImVec2 imageSize;
        ImTextureID imageID;

        //Set imageID and imageSize
        SetImageValues(filepath, imageID, imageSize);

        ImVec2 childSize = ImVec2(imageSize.x + ImGui::GetStyle().FramePadding.x * 2.f, imageSize.y + ImGui::CalcTextSize(filename.c_str()).y + ImGui::GetStyle().FramePadding.y * 15.f);

        if (mSelectedPath == mCurrPathArray[i].path())
        {
            ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.7f, 0.7f, 0.7f, 0.7f));
            mCanPop = true;
        }

        //If the mHoveredPath is the same as the path we're in we change it's style color
        else if (mHoveredPath == mCurrPathArray[i].path())
        {
            ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.5f, 0.5f, 0.5f, 0.5f));
            mCanPop = true;
        }

        ImGui::BeginChild(filename.c_str(), childSize, ImGuiChildFlags_None, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_AlwaysUseWindowPadding);
        ImGui::Image(imageID, imageSize);

        InteractionWithItems(mCurrPathArray[i]);

        DisplayText(mCurrPathArray[i], filename, imageSize);

        ImGui::EndChild();

        RightClickWindow(mCurrPathArray[i]);

        if (mCanPop)
        {
            ImGui::PopStyleColor();
            mCanPop = false;
        }

        ImGui::SameLine();

        if (ImGui::GetCursorPosX() > ImGui::GetContentRegionAvail().x * 2)
        {
            //Create a dummy if we can't create another child for a file/folder so it creates an empty space and go to a new line
            ImGui::Dummy(ImVec2(0, 0));
        }
    }

    if (!mSelectedPath.empty() && ImGui::IsKeyPressed(ImGuiKey_F2))
    {
        mRenamingPath = mSelectedPath;
    }

    if (!mIsAnythingHovered)
    {
        mHoveredPath = "";
    }


    mCurrPathArray.resize(0);
    mCurrPathArray.shrink_to_fit();
}

void ContentBrowser::DisplayWindow()
{
    ImGui::Begin("Content Browser");

    ImGui::BeginChild("Hierarchy", ImVec2(ImGui::GetContentRegionAvail().x * 0.5f, 0), ImGuiChildFlags_Border | ImGuiChildFlags_ResizeX);
    DisplayDirectories(mPath);
    ImGui::EndChild();

    ImGui::SameLine();
    ImGui::BeginChild("Repertory", ImVec2(0, 0), ImGuiChildFlags_Border);
    DisplayActualDirectory(mCurrentPath);
    ImGui::EndChild();

    ImGui::End();
}
