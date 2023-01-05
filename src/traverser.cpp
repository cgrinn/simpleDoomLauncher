#include "../head/traverser.h"
#include "../head/error.h"
#include <algorithm>

IWadTraverser::IWadTraverser(wxArrayString* shortRecord, wxArrayString* longRecord)
{
    sRecord = shortRecord;
    lRecord = longRecord;
}
 
//Checks the directory to traverse for the IWad filename, if it's there, it's added to lrecord and srecord
wxDirTraverseResult IWadTraverser::OnFile(const wxString& filename)
{
    if(filename.Right(4) == ".WAD")
    {
        lRecord->Add(filename);
        sRecord->Add(filename.AfterLast('/'));
    } 
    return wxDIR_CONTINUE;
}

wxDirTraverseResult IWadTraverser::OnDir(const wxString& WXUNUSED(dirname))
{
    return wxDIR_CONTINUE;
}

wxDirTraverseResult IWadTraverser::OnOpenError(const wxString& openerrorname)
{
    logError(openerrorname.ToStdString());
    return wxDIR_IGNORE;
}

PWadTraverser::PWadTraverser(wxArrayString* shortRecord, wxArrayString* longRecord)
{
    sRecord = shortRecord;
    lRecord = longRecord;
}

//Checks the directory to traverse for the pwad filename, if it's there, it's added to lrecord and srecord
wxDirTraverseResult PWadTraverser::OnFile(const wxString& filename)
{
    if(filename.Right(4) == ".wad")
    {
        lRecord->Add(filename);
        sRecord->Add(filename.AfterLast('/'));
    } 
    return wxDIR_CONTINUE;
}

wxDirTraverseResult PWadTraverser::OnDir(const wxString& WXUNUSED(dirname))
{
    return wxDIR_CONTINUE;
}

wxDirTraverseResult PWadTraverser::OnOpenError(const wxString& openerrorname)
{
    logError(openerrorname.ToStdString());
    return wxDIR_IGNORE;
}

ModTraverser::ModTraverser(wxArrayString* shortRecord, wxArrayString* longRecord)
{
    sRecord = shortRecord;
    lRecord = longRecord;
}
//Checks the directory to traverse for the mod filename, if it's there, it's added to lrecord and srecord
wxDirTraverseResult ModTraverser::OnFile(const wxString& filename)
{
    if(filename.Right(4) == ".pk3")
    {
        lRecord->Add(filename);
        sRecord->Add(filename.AfterLast('/'));
    } 
    return wxDIR_CONTINUE;
}

wxDirTraverseResult ModTraverser::OnDir(const wxString& WXUNUSED(dirname))
{
    return wxDIR_CONTINUE;
}

wxDirTraverseResult ModTraverser::OnOpenError(const wxString& openerrorname)
{
    logError(openerrorname.ToStdString());
    return wxDIR_IGNORE;
}

SourceTraverser::SourceTraverser(wxArrayString* shortRecord, wxArrayString* longRecord, std::vector<std::string>* sourceList, bool dotexe)
{
    sRecord = shortRecord;
    lRecord = longRecord;
    sourceNames = sourceList;
    sourceNameLength = sourceList->size();
    exe = dotexe;
}

//Checks the directory to traverse for the source filename, if it's there, it's added to lrecord and srecord
wxDirTraverseResult SourceTraverser::OnFile(const wxString& filename)
{
    for(int i = 0; i < sourceNameLength; i++)
    {
        if(filename.AfterLast('/') == sourceNames->at(i))
        {
            lRecord->Add(filename);
            sRecord->Add(filename.AfterLast('/'));
        } 
        if(exe && filename.AfterLast('/') == (sourceNames->at(i) + ".exe"))
        {
            lRecord->Add(filename);
            sRecord->Add(filename.AfterLast('/'));
        }
    }

    return wxDIR_CONTINUE;
}

wxDirTraverseResult SourceTraverser::OnDir(const wxString& WXUNUSED(dirname))
{
    return wxDIR_CONTINUE;
}

wxDirTraverseResult SourceTraverser::OnOpenError(const wxString& openerrorname)
{
    logError(openerrorname.ToStdString());
    return wxDIR_IGNORE;
}


