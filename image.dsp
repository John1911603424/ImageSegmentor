# Microsoft Developer Studio Project File - Name="image" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=image - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "image.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "image.mak" CFG="image - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "image - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "image - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/imagex", BAAAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "image - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "../HCE/gdaldll/include/" /I "C:/Program Files/OpenCV/cxcore/include" /I "C:/Program Files/OpenCV/cv/include" /I "C:/Program Files/OpenCV/otherlibs/highgui" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 gdal_i.lib cxcorel.lib cvl.lib highguil.lib /nologo /subsystem:windows /machine:I386 /libpath:"../HCE/gdaldll/lib" /libpath:"C:/Program Files/OpenCV/lib"
# SUBTRACT LINK32 /debug

!ELSEIF  "$(CFG)" == "image - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "../HCE/gdaldll/include/" /I "C:/Program Files/OpenCV/cxcore/include" /I "C:/Program Files/OpenCV/cv/include" /I "C:/Program Files/OpenCV/cvaux/include" /I "C:/Program Files/OpenCV/ml/include" /I "C:/Program Files/OpenCV/otherlibs/highgui" /I "C:/Program Files/OpenCV/otherlibs/cvcam/include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /FD /GZ /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 gdal_i.lib cxcorel.lib cvl.lib highguil.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept /libpath:"../HCE/gdaldll/lib" /libpath:"C:/Program Files/OpenCV/lib" /libpath:"E:\landcruiser\stickseg\Debug"
# SUBTRACT LINK32 /profile /pdb:none

!ENDIF 

# Begin Target

# Name "image - Win32 Release"
# Name "image - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\BandSelDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CannyPara.cpp
# End Source File
# Begin Source File

SOURCE=.\ChangeDetector.cpp
# End Source File
# Begin Source File

SOURCE=.\ChildFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\CIE.cpp
# End Source File
# Begin Source File

SOURCE=.\CKProcessTimeCounter.cpp
# End Source File
# Begin Source File

SOURCE=.\colormrf.cpp
# End Source File
# Begin Source File

SOURCE=.\DIB.cpp
# End Source File
# Begin Source File

SOURCE=.\fuzzylogic.cpp
# End Source File
# Begin Source File

SOURCE=.\HC.cpp
# End Source File
# Begin Source File

SOURCE=.\image.cpp
# End Source File
# Begin Source File

SOURCE=.\image.rc
# End Source File
# Begin Source File

SOURCE=.\imageDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\imageView.cpp
# End Source File
# Begin Source File

SOURCE=.\InitiateDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\mersenne.cpp
# End Source File
# Begin Source File

SOURCE=.\MFeatDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MMOFUNC.cpp
# End Source File
# Begin Source File

SOURCE=.\MorphDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\mrf_1.cpp
# End Source File
# Begin Source File

SOURCE=.\MRFOptimDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MultiDiff.cpp
# End Source File
# Begin Source File

SOURCE=.\MyColorSpace.cpp
# End Source File
# Begin Source File

SOURCE=.\MyGabor.cpp
# End Source File
# Begin Source File

SOURCE=.\MyMath.cpp
# End Source File
# Begin Source File

SOURCE=.\MyTexture.cpp
# End Source File
# Begin Source File

SOURCE=.\PreviewDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\QT.cpp
# End Source File
# Begin Source File

SOURCE=.\Region.cpp
# End Source File
# Begin Source File

SOURCE=.\ResultDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ResultImageDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SegApi.cpp
# End Source File
# Begin Source File

SOURCE=.\SegmenterMS.cpp
# End Source File
# Begin Source File

SOURCE=.\SelPropDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SetPropDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\SUSANDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Thinner.cpp
# End Source File
# Begin Source File

SOURCE=.\THRESHDlg.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\_MC.h
# End Source File
# Begin Source File

SOURCE=.\BandSelDlg.h
# End Source File
# Begin Source File

SOURCE=.\CannyPara.h
# End Source File
# Begin Source File

SOURCE=.\ChangeDetector.h
# End Source File
# Begin Source File

SOURCE=.\ChildFrm.h
# End Source File
# Begin Source File

SOURCE=.\CIE.h
# End Source File
# Begin Source File

SOURCE=.\CKProcessTimeCounter.h
# End Source File
# Begin Source File

SOURCE=.\colormrf.h
# End Source File
# Begin Source File

SOURCE=.\DIB.h
# End Source File
# Begin Source File

SOURCE=.\filter.h
# End Source File
# Begin Source File

SOURCE=.\fuzzylogic.h
# End Source File
# Begin Source File

SOURCE=.\GlobalApi.h
# End Source File
# Begin Source File

SOURCE=.\HC.h
# End Source File
# Begin Source File

SOURCE=.\image.h
# End Source File
# Begin Source File

SOURCE=.\imageDoc.h
# End Source File
# Begin Source File

SOURCE=.\imageinclude\ImageErrors.h
# End Source File
# Begin Source File

SOURCE=.\imageinclude\Imageload.h
# End Source File
# Begin Source File

SOURCE=.\imageView.h
# End Source File
# Begin Source File

SOURCE=.\ImpExpData.h
# End Source File
# Begin Source File

SOURCE=.\InitiateDlg.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\MFeatDlg.h
# End Source File
# Begin Source File

SOURCE=.\misc.h
# End Source File
# Begin Source File

SOURCE=.\MMOFUNC.h
# End Source File
# Begin Source File

SOURCE=.\MorphDlg.h
# End Source File
# Begin Source File

SOURCE=.\mrf_1.h
# End Source File
# Begin Source File

SOURCE=.\MRFOptimDlg.h
# End Source File
# Begin Source File

SOURCE=.\MultiDiff.h
# End Source File
# Begin Source File

SOURCE=.\MyColorSpace.h
# End Source File
# Begin Source File

SOURCE=.\MyGabor.h
# End Source File
# Begin Source File

SOURCE=.\MyMath.h
# End Source File
# Begin Source File

SOURCE=.\MyTexture.h
# End Source File
# Begin Source File

SOURCE=.\NP.h
# End Source File
# Begin Source File

SOURCE=.\PreviewDlg.h
# End Source File
# Begin Source File

SOURCE=.\QT.h
# End Source File
# Begin Source File

SOURCE=.\randomc.h
# End Source File
# Begin Source File

SOURCE=.\Region.h
# End Source File
# Begin Source File

SOURCE=.\require.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\ResultDlg.h
# End Source File
# Begin Source File

SOURCE=.\ResultImageDlg.h
# End Source File
# Begin Source File

SOURCE=".\segment-graph.h"
# End Source File
# Begin Source File

SOURCE=".\segment-image.h"
# End Source File
# Begin Source File

SOURCE=.\SegmenterMS.h
# End Source File
# Begin Source File

SOURCE=.\SelPropDlg.h
# End Source File
# Begin Source File

SOURCE=.\SetPropDlg.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\SUSANDlg.h
# End Source File
# Begin Source File

SOURCE=.\Thinner.h
# End Source File
# Begin Source File

SOURCE=.\ThreshDlg.h
# End Source File
# Begin Source File

SOURCE=.\timeshow.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\image.ico
# End Source File
# Begin Source File

SOURCE=.\res\image.rc2
# End Source File
# Begin Source File

SOURCE=.\res\imageDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\LENA.BMP
# End Source File
# Begin Source File

SOURCE=.\res\OriginIcon.ico
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
