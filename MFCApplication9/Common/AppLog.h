#pragma once
#include <stdio.h>
#include <stdarg.h>
#include <afxwin.h>
#include <direct.h>
class CLog
{
public:
	CLog()
	{
		m_bPopMsg = TRUE;
		m_pStdioFile = NULL;
		CString sFileName = L"";

		m_sFileName.Format(L"%s\\%s%s%03d.log", GetAppDir().Left(GetAppDir().ReverseFind(*L"\\")), sFileName, CTime::GetCurrentTime().Format(L"%Y%m%d_%H%M%S"),GetTickCount()%1000);
	};
	~CLog()
	{
		if (NULL != m_pStdioFile)
		{
			m_pStdioFile->Close();
			delete m_pStdioFile;
		}
	};
private:
	CString m_sFileName;
	CStdioFile* m_pStdioFile;
	BOOL m_bPopMsg;

	CString GetAppDir()
	{
		CString sTmp = L"";
		TCHAR sBuf[1024];

		GetModuleFileNameW(NULL, sBuf, MAX_PATH);

		sTmp = sBuf;
		return sTmp;
	};
	void SearchAppDirFiles(char sPath[1024],char sCompare[1024])
	{
		/*PWIN32_FIND_DATAA filedata{};
		if (FindFirstFileA(sPath, filedata) != nullptr)
		{
			while (filedata->cFileName != sCompare)
			{

			}
		}*/
	};
public:
	void SetFileName(CString fileName)
	{
		if (fileName.Find(L":") >= 0 )
		{
			m_sFileName = fileName;
		}
		else
		{
			m_sFileName = GetAppDir().Left(GetAppDir().ReverseFind(*L"\\")) + L"\\" + fileName;
		}
	};
	void WriteLog(char* format, ...)
	{
		CString sTmp=L"";
		va_list argptr;
		CString sFile = m_sFileName;


		try
		{
			if ( NULL == m_pStdioFile )
			{
				char str[1024];
				int nLen = sFile.GetLength();
				int delIndex = sFile.ReverseFind(*L"\\");
				sTmp = sFile.Left(delIndex + 1);
				wsprintfA(str, "%ls", sTmp.GetBuffer());

				//if (_mkdir(str) >= 0)
				{

					//CStdioFile xFile(sFile, CStdioFile::modeCreate | CStdioFile::modeWrite | CStdioFile::modeNoTruncate | CStdioFile::shareDenyNone);

					m_pStdioFile = new CStdioFile(m_sFileName, CStdioFile::modeCreate | CStdioFile::modeNoTruncate | CStdioFile::modeWrite | CStdioFile::shareDenyNone);

					//xFile.Open(m_sFileName, CStdioFile::modeCreate | CStdioFile::modeNoTruncate | CStdioFile::modeWrite | CStdioFile::shareDenyNone);
					//CStdioFile(m_sFileName, CStdioFile::modeCreate | CStdioFile::modeNoTruncate | CStdioFile::modeWrite | CStdioFile::shareDenyNone);
					//xFile.SeekToEnd();
					//vfprintf(xFile.m_pStream, format, argptr);
				}
			}

			if ( NULL != m_pStdioFile)
			{
				m_pStdioFile->SeekToEnd();

				va_start(argptr, format);
				vfprintf(m_pStdioFile->m_pStream, format, argptr);
				va_end(argptr);

			}


		}
		catch (CFileException* pFileException)
		{
			TCHAR sErrMsg[300];
			//sErrMsg.Format(L"Error:%d", pFileException->m_cause);
			pFileException->GetErrorMessage(sErrMsg, 200);
			pFileException->Delete();
			if (m_bPopMsg)
			{
				m_bPopMsg = FALSE;
				AfxMessageBox(sErrMsg);
			}

		}

		//xFile.Open(m_sFileName, CStdioFile::modeCreate | CStdioFile::modeNoTruncate | CStdioFile::modeWrite | CStdioFile::shareDenyNone);

	};
};