#pragma once

#include "stdafx.h"

#ifndef SIGNATURE
#define SIGNATURE uint
#endif

#ifndef SNORTID
#define URLID uint
#endif

struct FILEVISITOR
{
	std::vector<CString> &m_Files;
	FILEVISITOR(std::vector<CString> &files) : m_Files(files) {}
	void operator()(LPCTSTR lpFilePath)
	{
		m_Files.push_back(CString(lpFilePath));
	}
};

struct STRSPLDELI
{
	std::string m_strDelimiters;
	STRSPLDELI(char* strDelimiters)
		: m_strDelimiters(strDelimiters) {}
	bool operator()(char c)
	{
		return (-1 != m_strDelimiters.find(c));
	}
};

struct STRSPLRECV
{
	std::vector<std::string> &m_SubStrs;
	STRSPLRECV(std::vector<std::string> &subStrs)
		: m_SubStrs(subStrs) {}
	void operator()(std::string::const_iterator cBeg, std::string::const_iterator cEnd)
	{
		m_SubStrs.push_back(std::string(cBeg, cEnd));
	}
};

void EraseBlankPrefix(std::string &str);

struct EDGE
{
	URLID nSid;
	SIGNATURE Sig;
};

void WriteEdgesToFile(std::vector<EDGE> edges);
void DeleteRepeat(std::vector<EDGE> &edges);
void LogSignature(std::vector<std::string> urls, std::vector<EDGE> &edges);
void ReadRules(const std::string &strText, std::vector<std::string> &urls);
void ReadFiles(LPCTSTR lpFileName, std::vector<std::string> &urls);

template<typename _Iter, typename _DeliFn, typename _StrFn>
void Spliter(_Iter iBeg, _Iter iEnd, _DeliFn dfn, _StrFn pfn)
{
	_Iter iPrev = iBeg;
	for (_Iter i = iBeg; i != iEnd; ++i)
	{
		if (dfn(*i))
		{
			if (iPrev != i)
			{
				pfn(iPrev, i);
			}
			iPrev = i + 1;
		}
	}
	if (iPrev != iEnd)
	{
		pfn(iPrev, iEnd);
	}
}

template<typename _Fn>
void VisitFilesInPath(LPCTSTR lpPath, LPCTSTR lpExt, _Fn pfn)
{
	CString strPath(lpPath);
	if (strPath[strPath.GetLength() -1] != _T('\\'))
	{
		strPath += _T("\\");
	}
	strPath += lpExt;

	//Find files in specified path
	CFileFind ff;
	for (BOOL br = ff.FindFile(strPath); br;)
	{
		br = ff.FindNextFile();
		strPath = ff.GetFilePath();
		if (ff.IsDirectory())
		{
			CString strTemp = ff.GetFileTitle();
			if(ff.GetFileName() != _T("..") && ff.GetFileName() != _T("."))
			{
				VisitFilesInPath(strPath, lpExt,pfn);
			}
		}
		else
		{
			pfn(strPath);
		}
	}
}