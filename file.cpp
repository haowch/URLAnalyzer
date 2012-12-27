#include "file.h"

void EraseBlank(std::string &str)
{
	for(; str.size() > 0; )
	{
		if (str[0] == '\t' || str[0] == ' ' || str[0] == '\r' || str[0] == '\n')
		{
			str.erase(str.begin());
		}
		else
		{
			break;
		}
	}
	size_t lastindex;
	for (; str.size() > 0; )
	{
		lastindex = str.size() - 1;
		if (str[lastindex] == '\t' || str[lastindex] == ' ' || str[lastindex] == '\r' || str[lastindex] == '\n')
		{
			str.erase(str.begin() + str.size() - 1);
		}
		else
		{
			break;
		}
	}
}

void WriteEdgesToFile(std::vector<EDGE> edges)
{
	std::ofstream fout("C:\\URLResults\\Edges.txt", std::ios::binary);
	size_t nCount = edges.size();
	fout.write((char*)&nCount, 4);
	for (std::vector<EDGE>::iterator i = edges.begin(); i != edges.end(); ++i)
	{
		fout.write((char*)&i->nSid, 4);
		fout.write((char*)&i->Sig, 4);
	}
	fout.close();
}

void DeleteRepeat(std::vector<EDGE> &edges)
{
	for (std::vector<EDGE>::iterator i = edges.begin(); i != edges.end(); ++i)
	{
		for (std::vector<EDGE>::iterator j = i + 1; j != edges.end();)
		{
			if (i->nSid == j->nSid)
			{
				if (i->Sig == j->Sig)
				{
					j = edges.erase(j);
				}
				else
				{
					++j;
				}
			}
			else
			{
				break;
			}
		}
	}
}

void LogSignature(std::vector<std::string> urls, std::vector<EDGE> &edges)
{
	const int n = 4;
	EDGE oneEDGE;
	URLID id = 0;
	unsigned char c[4];
	for (std::vector<std::string>::iterator i = urls.begin(); i != urls.end(); ++i)
	{
		++id;
		if (i->size() >= 4)
		{
			oneEDGE.nSid = id;
			for (size_t j = 0; j + n <= i->size(); ++j)
			{
				for (size_t k = j; k < j + n; ++k)
				{
					c[k - j] = (*i)[k];
				}
				oneEDGE.Sig = *(size_t*)c;
				edges.push_back(oneEDGE);
			}
		}
	}
}

void ReadURLs(const std::string strText, std::vector<std::string> &urls)
{
	Spliter(strText.begin(), strText.end(), STRSPLDELI("\n"), STRSPLRECV(urls));

	for (std::vector<std::string>::iterator i = urls.begin(); i != urls.end(); ++i)
	{
		EraseBlank(*i);
	}
}

void ReadFiles(LPCTSTR lpFileName, std::vector<std::string> &urls)
{
	std::string strPfContent;
	CFile bmpFile;

	if (!bmpFile.Open(lpFileName, CFile::modeRead | CFile::shareDenyWrite))
	{
		std::cerr << "Open file failed!" << std::endl;
	}

	DWORD dwFileSize = (DWORD)bmpFile.GetLength();

	char *pBuf = new char[dwFileSize + 1];
	ZeroMemory(pBuf, dwFileSize + 1);
	bmpFile.Read(pBuf, dwFileSize);
	strPfContent = pBuf;
	ReadURLs(strPfContent, urls);
	delete[] pBuf;
}