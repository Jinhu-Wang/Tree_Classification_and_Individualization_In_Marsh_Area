========================================================================
    STATIC LIBRARY : LASlib Project Overview
========================================================================

AppWizard has created this LASlib library project for you.

No source files were created as part of your project.


LASlib.vcxproj
    This is the main project file for VC++ projects generated using an Application Wizard.
    It contains information about the version of Visual C++ that generated the file, and
    information about the platforms, configurations, and project features selected with the
    Application Wizard.

LASlib.vcxproj.filters
    This is the filters file for VC++ projects generated using an Application Wizard. 
    It contains information about the association between the files in your project 
    and the filters. This association is used in the IDE to show grouping of files with
    similar extensions under a specific node (for e.g. ".cpp" files are associated with the
    "Source Files" filter).
	
/////////////////////////////////////////////////////////////////////////////
Other notes:

AppWizard uses "TODO:" comments to indicate parts of the source code you
should add to or customize.

/////////////////////////////////////////////////////////////////////////////


================================================================================================================	
	1） 下载源码后，解压源码到任意目录。
	2） 用VS 2015，打开解压后文件夹中LASlib目录下的LASlib.dsw工程文件。提示需要进行版本升级转换。点击确认进行转换。
	3） 右键点击LASlib工程，在属性对话框中选择配置管理器 按钮，新建x64的配置。同时，设置当前配置为x64。 
	4） 右键点击LASlib工程，在属性对话框 - C/C++ -预处理器下增加LASZIPDLL_EXPORTS预编译宏。
	5）右键点击LASlib工程，在属性对话框 -C/C++ -常规下-附加包含目录中删除stl项。
	6） 打开LASlib工程下的头文件mydefs.hpp，进行如下修改。
		//#if defined(_MSC_VER) && (_MSC_VER < 1300)
		//typedef int                BOOL;
		//#else
		//typedef bool               BOOL;
		//#endif
		typedef int                BOOL;
	7）编译相应的工程文件。最终得到基于VS 2015平台的64位开发库。

	Jinhu Wang 2017.09.29 @TU Delft.
================================================================================================================	
