/***********************************************************************************
 * Copyright (c) 2013-2015 "Thomas Hoegg et al."
 * [http://gu-dsl.org]; information about commercial use mail at info@gu-dsl.org
 *
 * This file is part of GU-DSL.
 *
 * GU-DSLis free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 **********************************************************************************/


#include "Misc/ProcessHandling.h"
#include <QProcess>

#if WIN32
#include <windows.h>
#include <Psapi.h>
#include <tchar.h>
#endif

QStringList CarWash3D::Process::ProcessHandling::getAllProcesses()
{
	unsigned long aProcesses[1024], cbNeeded, cProcesses;

	if(!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded))
		return QStringList();

	QStringList lprocess;
	cProcesses = cbNeeded / sizeof(unsigned long);

	for(unsigned int i = 0; i < cProcesses; i++)
	{
		if(aProcesses[i] == 0)
			continue;

		lprocess.append(getProcessName(aProcesses[i]));
	}

	return lprocess;
}

QString CarWash3D::Process::ProcessHandling::getProcessName(long pid)
{
	TCHAR szProcessName[MAX_PATH] = _T("<unknown>");

	// Get a handle to the process.  
	HANDLE hProcess = OpenProcess( PROCESS_QUERY_INFORMATION |
		PROCESS_VM_READ,
		FALSE, pid );

	// Get the process name.  
	if (NULL != hProcess )
	{
		HMODULE hMod;
		DWORD cbNeeded;

		//Given a handle to a process, this returns all the modules running within the process.
		//The first module is the executable running the process,
		//and subsequent handles describe DLLs loaded into the process.
		if ( EnumProcessModules( hProcess, &hMod, sizeof(hMod), &cbNeeded) )
		{
			//This function returns the short name for a module,
			//typically the file name portion of the EXE or DLL
			GetModuleBaseName( hProcess, hMod, szProcessName,
				sizeof(szProcessName)/sizeof(TCHAR) );
		}
	}


	//close the process handle
	CloseHandle( hProcess );

#if UNICODE
	return QString::fromWCharArray(szProcessName);
#else
	return QString(szProcessName);
#endif
}

bool CarWash3D::Process::ProcessHandling::isProcessRunning( QString processName )
{
	QStringList processes = getAllProcesses();
	return processes.contains(processName);
}



