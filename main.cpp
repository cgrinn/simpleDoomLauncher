#include <wx/wx.h>
#include <string>
#include "head/error.h"
#include "head/traverser.h"
#include <iostream>
#include <wx/config.h>
#include <wx/string.h>

std::vector<std::string> gSourceNames = {"gzdoom", "dsda-doom", "prboom", "prboom-plus", "zdoom", "chocolate-doom", "crispy-doom", "lzdoom"};


class MainApp: public wxApp
{
    public:
    virtual bool OnInit();
    private:

};

class MainFrame: public wxFrame
{
    public:
    MainFrame(wxString title, wxPoint pos, wxSize size);
    wxButton* exitButton;
    wxButton* doomButton;
    wxButton* addWadDirButton;
    wxButton* removeWadDirButton;
    wxButton* scanSourceButton;
    wxButton* manualSourceButton;
    wxButton* removeSourceButton;


    wxListBox* iwadList;
    wxListBox* pwadList;
    wxListBox* modList;
    wxListBox* wadDirList;
    wxListBox* sourceList;

    wxStaticText* iwadLabel;
    wxStaticText* pwadLabel;
    wxStaticText* modLabel;
    wxStaticText* wadDirLabel;
    wxStaticText* sourceLabel;
    wxStaticText* compLevelLabel;
    wxStaticText* skillLabel;
    wxStaticText* levelLabel;
    wxStaticText* argsLabel;

    wxCheckBox* noMonstersBox;
    wxCheckBox* fastMonstersBox;
    wxCheckBox* respawnMonstersBox;
    wxCheckBox* musicBox;

    wxChoice* compLevelBox;
    wxChoice* skillBox;

    wxTextEntry* levelBox;
    wxTextEntry* argsBox;

    private:
    wxArrayString wadDirs;
    wxArrayString iwads;
    wxArrayString iwadsFullPath;
    wxArrayString pwads;
    wxArrayString pwadsFullPath;
    wxArrayString mods;
    wxArrayString modsFullPath;
    wxArrayString sources;
    wxArrayString sourcesFullPath;
    wxArrayString manualSources;

    void onExit(wxCommandEvent& event);
    void onClose(wxCloseEvent& event);
    void startDoom(wxCommandEvent& event);
    void clearSelection(wxCommandEvent& event);
    void closeWadDir(wxCommandEvent& event);
    void openWadDir(wxCommandEvent& event);
    void manualSource(wxCommandEvent& event);
    void removeSource(wxCommandEvent& event);
    void scanSource(wxCommandEvent& event);

    void addWadDir(std::string dir);
    void removeWadDir(int removeAt);
    void searchWads();
    void populateWads();
    void populateSource();
    void autoSource();
    void searchSource(wxString directory, bool exe = false);
    void addSource(wxString path, wxString pathFull);

    void saveToIni();
    void loadFromIni();
};

wxIMPLEMENT_APP(MainApp);

bool MainApp::OnInit()
{   
    MainFrame* frame = new MainFrame(wxString("Simple Doom Launcher"), wxDefaultPosition, wxSize(560, 650));
    frame->Show(true); 
    SetTopWindow(frame);
    return true;
}

enum {ID_DOOMSTART = wxID_HIGHEST + 1, ID_ADDWADDIR = wxID_HIGHEST + 2, ID_REMOVEWADDIR = wxID_HIGHEST+3, ID_SCANSOURCE = wxID_HIGHEST+4, ID_REMOVESOURCE = wxID_HIGHEST+5, ID_MANUALSOURCE = wxID_HIGHEST+6};

MainFrame::MainFrame(wxString title, wxPoint pos, wxSize size):wxFrame(nullptr, wxID_ANY, title, pos, size)
{
    //Initalises controls, sets default values and binds events 

    //Exit Button is unused
    //exitButton = new wxButton(this, wxID_EXIT, "Exit", wxPoint(25,25), wxSize(100, 50));
    doomButton = new wxButton(this, ID_DOOMSTART, "Play Doom", wxPoint(385,520), wxSize(165, 90));
    removeWadDirButton = new wxButton(this, ID_REMOVEWADDIR, "Del Directory", wxPoint(450,180), wxSize(100, 30));
    addWadDirButton = new wxButton(this, ID_ADDWADDIR, "Add Directory", wxPoint(340,180), wxSize(100, 30));
    scanSourceButton = new wxButton(this, ID_SCANSOURCE, "Scan Sources", wxPoint(10,180), wxSize(100, 30));
    removeSourceButton = new wxButton(this, ID_REMOVESOURCE, "Remove Source", wxPoint(120,180), wxSize(100, 30));
    manualSourceButton = new wxButton(this, ID_MANUALSOURCE, "Add Source", wxPoint(230,180), wxSize(100, 30));

    //Exit Button is unused
    //Bind(wxEVT_BUTTON, &MainFrame::onExit, this, wxID_EXIT);
    Bind(wxEVT_BUTTON, &MainFrame::startDoom, this, ID_DOOMSTART);
    Bind(wxEVT_BUTTON, &MainFrame::closeWadDir, this, ID_REMOVEWADDIR);
    Bind(wxEVT_BUTTON, &MainFrame::openWadDir, this, ID_ADDWADDIR);
    Bind(wxEVT_BUTTON, &MainFrame::scanSource, this, ID_SCANSOURCE);
    Bind(wxEVT_BUTTON, &MainFrame::removeSource, this, ID_REMOVESOURCE);
    Bind(wxEVT_BUTTON, &MainFrame::manualSource, this, ID_MANUALSOURCE);
    Bind(wxEVT_CLOSE_WINDOW, &MainFrame::onClose, this);


    noMonstersBox = new wxCheckBox(this, wxID_ANY, "No monsters", wxPoint(385,235), wxSize(150,25));
    fastMonstersBox = new wxCheckBox(this, wxID_ANY, "Fast monsters", wxPoint(385,265), wxSize(150,25));
    respawnMonstersBox = new wxCheckBox(this, wxID_ANY, "Respawn monsters", wxPoint(385,295), wxSize(150,25));
    musicBox = new wxCheckBox(this, wxID_ANY, "No music", wxPoint(385,325), wxSize(150,25));

    compLevelLabel = new wxStaticText(this, wxID_ANY, "Comp Level", wxPoint(385, 355), wxSize(145, 20));
    skillLabel = new wxStaticText(this, wxID_ANY, "Skill", wxPoint(385,410), wxSize(145, 20));
    levelLabel = new wxStaticText(this, wxID_ANY, "Level", wxPoint(385, 465), wxSize(145, 20));

    wxString compArray[23];
    compArray[0] = "Default";
    for (int i = 0; i <= 21; i++)
    {
        compArray[i+1] = std::to_string(i);
    }
    compLevelBox = new wxChoice(this, wxID_ANY, wxPoint(385,375), wxSize(125, 25), 23, &compArray[0]);
    compLevelBox->SetSelection(0);

    wxString skillArray[6] = {"None", "ITYTD", "HNTR", "HMP", "UV", "NM"};
    skillBox = new wxChoice(this, wxID_ANY, wxPoint(385,430), wxSize(125, 25), 6, &skillArray[0]);
    skillBox->SetLabel("Difficulty");
    skillBox->SetSelection(3);


    wxTextValidator numFilter(wxFILTER_DIGITS);
    levelBox = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxPoint(385,485), wxSize(125, 25), 0, numFilter, "Map");
    levelBox->SetMaxLength(3);

    iwadLabel = new wxStaticText(this, wxID_ANY, "IWADS", wxPoint(200, 410), wxSize(125, 20));
    pwadLabel = new wxStaticText(this, wxID_ANY, "PWADS", wxPoint(10, 215), wxSize(125, 20));    
    modLabel = new wxStaticText(this, wxID_ANY, "MODS", wxPoint(200, 215), wxSize(125, 20));
    wadDirLabel = new wxStaticText(this, wxID_ANY, "Wad and Mod Search Directories", wxPoint(200, 5), wxSize(145, 20));
    sourceLabel = new wxStaticText(this, wxID_ANY, "Source Ports", wxPoint(5, 5), wxSize(145, 20));

    iwadList = new wxListBox(this, wxID_ANY, wxPoint(200, 435), wxSize(175, 150), 0, NULL, wxLB_SINGLE| wxLB_ALWAYS_SB);
    pwadList = new wxListBox(this, wxID_ANY, wxPoint(10, 235), wxSize(175, 350), 0, NULL, wxLB_MULTIPLE | wxLB_ALWAYS_SB);
    modList = new wxListBox(this, wxID_ANY, wxPoint(200, 235), wxSize(175, 165), 0, NULL, wxLB_MULTIPLE | wxLB_ALWAYS_SB);
    wadDirList = new wxListBox(this, wxID_ANY, wxPoint(200, 25), wxSize(350, 145), 0, NULL, wxLB_SINGLE | wxLB_ALWAYS_SB);
    sourceList = new wxListBox(this, wxID_ANY, wxPoint(10, 25), wxSize(175, 145), 0, NULL, wxLB_SINGLE | wxLB_ALWAYS_SB);

    argsLabel = new wxStaticText(this, wxID_ANY, "Args:", wxPoint(10, 590), wxSize(40, 20));
    argsBox = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxPoint(55, 590), wxSize(320, 20));


    //Check for default source ports
    autoSource();
    populateSource();

    //Finally load saved values, overwritting any defaults 
    loadFromIni();
}

void MainFrame::onExit(wxCommandEvent& event)
{
    Close(true);
}

//Saves form varaibles and then exits
void MainFrame::onClose(wxCloseEvent& event)
{
    saveToIni();
    Destroy();
}


//Deletes old saved doomLauchConf and Writes form variables to doomLauchConf    
void MainFrame::saveToIni()
{
    wxConfig conf("doomLaunchConf");

    conf.DeleteAll();

    conf.Write("sourceSel", sourceList->GetSelection());
    conf.Write("iWadSel", iwadList->GetSelection());
    
    conf.Write("noMon", noMonstersBox->GetValue());
    conf.Write("fasMon", fastMonstersBox->GetValue());
    conf.Write("resMon", respawnMonstersBox->GetValue());
    conf.Write("noMus", musicBox->GetValue());

    conf.Write("skill", skillBox->GetSelection());
    conf.Write("compLevel", compLevelBox->GetSelection());
    conf.Write("level", levelBox->GetValue());
    conf.Write("args", argsBox->GetValue());

    conf.SetPath("/wadDir");
    for (int i = 0; i < wadDirs.GetCount(); i++)
    {
        conf.Write(std::to_string(i), wadDirs[i]);
    }

    conf.SetPath("/pwad");
    wxArrayInt selections;
    pwadList->GetSelections(selections);
    for (int i = 0; i < selections.GetCount(); i++)
    {
        conf.Write(std::to_string(i), selections[i]);
    }

    conf.SetPath("/");

}

//Reads variables from doomLauchConf and applies them to form controls, if it can
void MainFrame::loadFromIni()
{
    wxConfig conf("doomLaunchConf");

    long lStore;
    wxString strStore; 
    if(conf.Read("sourceSel", &strStore))
    {
        strStore.ToLong(&lStore);
        sourceList->SetSelection(lStore);
    }
    if(conf.Read("noMon", &strStore))
    {
        strStore.ToLong(&lStore);
        noMonstersBox->SetValue(lStore);
    }
    if(conf.Read("fasMon", &strStore))
    {
        strStore.ToLong(&lStore);
        fastMonstersBox->SetValue(lStore);
    }
    if(conf.Read("resMon",&strStore))
    {
        strStore.ToLong(&lStore);
        respawnMonstersBox->SetValue(lStore);
    }
    if(conf.Read("noMus",&strStore))
    {
        strStore.ToLong(&lStore);
        musicBox->SetValue(lStore);
    }
    if(conf.Read("skill",&strStore))
    {
        strStore.ToLong(&lStore);
        skillBox->SetSelection(lStore);
    }
    if(conf.Read("compLevel",&strStore))
    {
        strStore.ToLong(&lStore);
        compLevelBox->SetSelection(lStore);
    }
    if(conf.Read("level",&strStore))
    {
        levelBox->WriteText(strStore);
    }
    if(conf.Read("args",&strStore))
    {
        argsBox->WriteText(strStore);
    }

    //Reads group of wad directories if they exist, storing them in wadDirList
    conf.SetPath("/wadDir");  
    bool cont = conf.GetFirstEntry(strStore, lStore);
    if(cont)
    {
        wadDirs.Clear();
    }
    while(cont)
    {
        addWadDir(conf.Read(strStore).ToStdString());
        cont = conf.GetNextEntry(strStore, lStore);
    }
    conf.SetPath("/");

    //Reads group of selected IWADs if they exist, storing them in iWadList
    if(conf.Read("iWadSel", &strStore))
    {
        strStore.ToLong(&lStore);
        if(iwadList->GetCount()>= lStore)
        {
            iwadList->SetSelection(lStore);
        }
        else
        {
            logError("Couldn't select iwad from conf file.");
        }
    }

    //Reads group of selected pWads if they exist, storing them in pWadList
    wxString valString;
    long valLong;
    conf.SetPath("/pwad");  
    cont = conf.GetFirstEntry(strStore, lStore);
    while(cont)
    {
        valString = conf.Read(strStore);
        valString.ToLong(&valLong);
        pwadList->SetSelection(valLong);
        cont = conf.GetNextEntry(strStore, lStore);
    }
    conf.SetPath("/");
}

//Uses the values of the form controls to contruct a string, then sends that string as a command to lauch doom
void MainFrame::startDoom(wxCommandEvent& event)
{   
    int sourceInt = sourceList->GetSelection();

    if(sourceInt == wxNOT_FOUND)
    {
        logError("Tried to lauch without a source port.");
        return;
    }

    std::string executeStr = sourcesFullPath.Item(sourceInt).ToStdString() + " -iwad ";
    wxArrayInt iwadArray = wxArrayInt();
    iwadList->GetSelections(iwadArray);
    if(iwadArray.GetCount() == 0)
    {
        logError("Tried to lauch without an IWAD.");
        return;
    }
    for(int i =0; i < iwadArray.GetCount() ; i++)
    {
        executeStr += "\"" + iwadsFullPath.Item(iwadArray[i]) + "\" ";
    }

    wxArrayInt pwadArray = wxArrayInt();
    wxArrayInt modArray = wxArrayInt();
    pwadList->GetSelections(pwadArray);
    modList->GetSelections(modArray);
    if(pwadArray.GetCount() != 0 || modArray.GetCount() != 0)
    {
        executeStr += "-file "; 
    }
    for(int i =0; i < pwadArray.GetCount() ; i++)
    {
        executeStr += "\"" + pwadsFullPath.Item(pwadArray[i]) + "\" ";
    }
    for(int i =0; i < modArray.GetCount() ; i++)
    {
        executeStr += "\"" + modsFullPath.Item(modArray[i]) + "\" ";
    }

    int compLevelSel= compLevelBox->GetSelection();
    if(compLevelSel != wxNOT_FOUND && compLevelSel != 0)
    {
        executeStr += "-complevel " + std::to_string(compLevelSel-1) + " ";
    }

    int skillLevelSel = skillBox->GetSelection();
    if(skillLevelSel != 0)
    {
        executeStr += "-skill " + std::to_string(skillLevelSel) + " ";
    }

    wxString mapSel = levelBox->GetValue();
    if(!mapSel.IsNull())
    {
        executeStr += "-warp " + mapSel.ToStdString() + " ";
    }

    if(noMonstersBox->GetValue())
    {
        executeStr += "-nomonsters ";
    }
    if(fastMonstersBox->GetValue())
    {
        executeStr += "-fast ";
    }
    if(respawnMonstersBox->GetValue())
    {
        executeStr += "-respawn ";
    }
    if(musicBox->GetValue())
    {
        executeStr += "-nomusic ";
    }
    wxString argValue = argsBox->GetValue();
    if(argValue != wxEmptyString)
    {
        executeStr += argValue;
    }

    system(executeStr.c_str());
}

//Adds dir to wadDirs, then refreshes wadDirList, wads and mods using the new dir
void MainFrame::addWadDir(std::string dir)
{
    wadDirs.Add(dir);
    wadDirList->Clear();
    if(wadDirs.GetCount() > 0)
    {
        wadDirList->InsertItems(wadDirs, 0);
    }
    searchWads();
    populateWads();
}

//Removes dir from wadDirs, then refreshes wadDirList, wads and mods to reflect the removal
void MainFrame::removeWadDir(int removeAt)
{
    if(removeAt < wadDirs.GetCount())
    {
        wadDirs.RemoveAt(removeAt);
        wadDirList->Clear();
        if(wadDirs.GetCount() > 0)
        {
            wadDirList->InsertItems(wadDirs, 0);
        }
        searchWads();
        populateWads();
    }
    else
    {
        logError("Couldn't find directory to remove.");
    }

}

//Opens a file dialogue asking for a directory to add to wad directories
void MainFrame::openWadDir(wxCommandEvent& event)
{
    wxDirDialog dirDialog(this, "Please select a directory to add.", wxEmptyString, wxDD_DEFAULT_STYLE);
    if(dirDialog.ShowModal()  == wxID_CANCEL)
    {
        return;
    }
    addWadDir(dirDialog.GetPath().ToStdString());   
}

//Removes selected dir
void MainFrame::closeWadDir(wxCommandEvent& event)
{
    int sel = wadDirList->GetSelection();
    if(sel != wxNOT_FOUND)
    {
        removeWadDir(sel);
    }
}

//Clears lists of wads and mods, then seaches stored wad dirs for any matching files, reconstructing them
void MainFrame::searchWads()
{
    iwads.clear();
    pwads.clear();
    mods.clear();

    wxDir dir;
    IWadTraverser iTrav(&iwads, &iwadsFullPath);
    PWadTraverser pTrav(&pwads, &pwadsFullPath);
    ModTraverser mTrav(&mods, &modsFullPath);

    for (int i = 0; i < wadDirs.GetCount(); i++)
    {
        dir.Open(wadDirs[i]);
        if(dir.IsOpened()) 
        {
            dir.Traverse(iTrav, wxEmptyString, wxDIR_FILES);
            dir.Traverse(pTrav, wxEmptyString, wxDIR_FILES);
            dir.Traverse(mTrav, wxEmptyString, wxDIR_FILES);
            dir.Close();
        }
        else
        {
            logError("Failed to open directory for wad search.");
        }
    }
}

//Populates list controls from stored wads and mods
void MainFrame::populateWads()
{
    iwadList->Clear();
    pwadList->Clear();
    modList->Clear();
    if(iwads.GetCount() > 0 )
    {
        iwadList->InsertItems(iwads, 0);
    } 

    if(pwads.GetCount() > 0 )
    {
        pwadList->InsertItems(pwads, 0);
    } 

    if(mods.GetCount() > 0 )
    {
        modList->InsertItems(mods, 0);
    } 
}

//Populates source list from stored sources
void MainFrame::populateSource()
{
    sourceList->Clear();
    if(sources.GetCount() > 0)
    {
        sourceList->InsertItems(sources, 0);
    }
}

//Opens a file dialouge asking for a source port to add
void MainFrame::manualSource(wxCommandEvent& event)
{
    wxFileDialog fileDialog(this, "Please select a manual source.", wxEmptyString, wxEmptyString, wxFileSelectorDefaultWildcardStr, wxFD_OPEN | wxFD_FILE_MUST_EXIST);
    if(fileDialog.ShowModal()  == wxID_CANCEL)
    {
        return;
    }
    manualSources.Add(fileDialog.GetPath());
}

//Removes the selected source port and if successful repopulates the source list control
void MainFrame::removeSource(wxCommandEvent& event)
{
    int sel = sourceList->GetSelection();
    if(sel == wxNOT_FOUND) 
    {
        logError("No source selected to remove.");
        return;
    }
    sources.RemoveAt(sel);
    sourcesFullPath.RemoveAt(sel);
    populateSource();
}

//Calls auto source scan and then populates the source list
//Probably superflous
void MainFrame::scanSource(wxCommandEvent& event)
{
    autoSource();
    populateSource();
}

//Checks the users operating system to call search source with appropiate directory
void MainFrame::autoSource()
{
    #ifdef __linux__
        searchSource("/usr/bin/");
    #elif _WIN32
        searchSource(wxStandardPaths::Get()->GetExecutablePath(), true);
    #endif
}

//Searches the given directory for know source ports, then adds them to sources
void MainFrame::searchSource(wxString directory, bool exe)
{
    wxDir dir(directory);
    wxArrayString tempSource, tempFullSource;
    SourceTraverser trav(&tempSource, &tempFullSource, &gSourceNames, exe);
    dir.Traverse(trav, wxEmptyString, wxDIR_FILES);
    for(int i = 0; i < tempSource.GetCount(); i++)
    {
        addSource(tempSource[i], tempFullSource[i]);
    }
    dir.Close();
}

//Add the short and full path to sources
void MainFrame::addSource(wxString path, wxString pathFull)
{   
    bool alreadyIn = false;
    for (int i = 0; i < sources.GetCount(); i++)
    {
        if(path == sources[i])
        {
            alreadyIn = true;
            break;
        }
    }
    if(!alreadyIn)
    {
        sources.Add(path);
        sourcesFullPath.Add(pathFull);
    }
}

