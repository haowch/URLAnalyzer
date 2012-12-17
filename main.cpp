#include "stdafx.h"
#include "file.h"

void Start(std::vector<CString> &prjFiles)
{
	std::vector<std::string> urls;

	for (size_t i = 0; i < prjFiles.size(); ++i)
	{
		std::cout << CStringA(prjFiles[i]) << std::endl;
		ReadFiles(prjFiles[i], urls);
	}
	std::cout << "Read files complete!" << std::endl;

	std::vector<EDGE> edges;

	LogSignature(urls, edges);

	std::cout << "LogSignature complete!" << std::endl;

	DeleteRepeat(edges);

	std::cout << "DeleteRepeat complete!" << std::endl;
	
	WriteEdgesToFile(edges);

	std::cout << "WriteEdgesToFile complete!" << std::endl;
}


void main(int nArgs, char **pArgs)
{
	std::vector<CString> prjFiles;

	if (nArgs != 3)
	{
		return;
	}

	CString strPath(pArgs[1]);
	CString strExt(pArgs[2]);

	VisitFilesInPath(strPath, strExt, FILEVISITOR(prjFiles));

	Start(prjFiles);

	system("pause");
}