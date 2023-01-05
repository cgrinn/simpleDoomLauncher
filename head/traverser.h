#pragma once
#include <wx/dir.h>
#include <wx/arrstr.h>
#include <string>


class IWadTraverser: public wxDirTraverser
{
    public:
    IWadTraverser(wxArrayString* shortRecord, wxArrayString* longRecord);
    virtual wxDirTraverseResult OnFile(const wxString& filename);
    virtual wxDirTraverseResult OnDir(const wxString& WXUNUSED(dirname));
    virtual wxDirTraverseResult OnOpenError(const wxString& openerrorname);

    wxArrayString* sRecord;
    wxArrayString* lRecord;

};

class PWadTraverser: public wxDirTraverser
{
    public:
    PWadTraverser(wxArrayString* shortRecord, wxArrayString* longRecord);
    virtual wxDirTraverseResult OnFile(const wxString& filename);
    virtual wxDirTraverseResult OnDir(const wxString& WXUNUSED(dirname));
    virtual wxDirTraverseResult OnOpenError(const wxString& openerrorname);

    wxArrayString* sRecord;
    wxArrayString* lRecord;
};

class ModTraverser: public wxDirTraverser
{
    public:
    ModTraverser(wxArrayString* shortRecord, wxArrayString* longRecord);
    virtual wxDirTraverseResult OnFile(const wxString& filename);
    virtual wxDirTraverseResult OnDir(const wxString& WXUNUSED(dirname));
    virtual wxDirTraverseResult OnOpenError(const wxString& openerrorname);

    wxArrayString* sRecord;
    wxArrayString* lRecord;
};

class SourceTraverser:public wxDirTraverser
{
    public:
    SourceTraverser(wxArrayString* shortRecord, wxArrayString* longRecord, std::vector<std::string>* sourceList, bool dotexe = false);
    virtual wxDirTraverseResult OnFile(const wxString& filename);
    virtual wxDirTraverseResult OnDir(const wxString& WXUNUSED(dirname));
    virtual wxDirTraverseResult OnOpenError(const wxString& openerrorname);

    wxArrayString* sRecord;
    wxArrayString* lRecord;
    int sourceNameLength;
    bool exe;
    std::vector<std::string>* sourceNames;
};