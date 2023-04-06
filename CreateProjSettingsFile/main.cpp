/***************************************************************
*   Purpose:    1-argv[1] (SettingValuesFile)
*              shall contain the absolute file path
*              and respective file name that contains settings
*              of a project.
*
*               2-Output - projSettingsFile
*              the file name will be constructed concatenating
*              home_dir + sub_dir + settings_dir + project_name + .txt
*              that will be read from the file informed in argv[1]
*              and will receive the settings of a project.
*              That file will be created if it does not exist
*              and will be populated by the parameters read from the
*              same file in argv[1].
*
***************************************************************/

#include "createProjSettingsFile_Proj.h"

#include "elapsedtime_Lib.h"
#include "openFile_Lib.h"
#include "logger_lib.h"

/********************************************************
*          helper functions
********************************************************/

bool
checkParameter( const QString & fileName, const QString & project = QString("CreateProjSettingsFile") );

/********************************************************
*          main function
********************************************************/
int
main( int argc, char *argv[] )
  {
    const QString logType = "RELEASE";
    QString proj {"CreateProjSettingsFile"};

    Logger_Lib::init(proj, logType );

  ElapsedTime_Lib   timeIntervalObj;

  /********************************************************
  *          verify if parameter informed
  ********************************************************/

  if ( argc < 2 )
     {
     qWarning()   << "(" << proj << ")"
                << "Please inform:"
                << Qt::endl
                << "The file (absolute file path) that contains The Project Settings "
                << Qt::endl
                << "in the form of Key/Value pairs like a Map."
                << Qt::endl
                << "Program will terminate"
                << Qt::endl;

     return 1;
     }

  /********************************************************
  *          check parameter
  ********************************************************/

  QString                   SettingValuesFile   = argv[ 1 ];

  bool                      isSettingValues_Ok  = checkParameter( argv[ 1 ] );

  if ( ! isSettingValues_Ok )
    return EXIT_FAILURE;

  /********************************************************
  *          starts treating input file
  ********************************************************/

  OpenFile_Lib              SettingValuesFile_In( SettingValuesFile, proj, "ROTX" );
  QTextStream               mySettingValuesFile_In( SettingValuesFile_In.getMp_File() );

  QString                   currentLine;

  QString                   mapKey;
  QString                   mapValue;

  int                       readCount     { 0 };
  int                       writeCount    { 0 };

  QMap  <QString, QString>  settingValuesMap;

  /********************************************************
  *       reads file and construct a Map
  ********************************************************/

  while ( ! mySettingValuesFile_In.atEnd() )
        {
        currentLine = mySettingValuesFile_In.readLine();

        // ignore comment lines (starts with char '#')
        if ( currentLine.startsWith( '#' ) )
          continue;

        ++readCount;

        if ( readCount == 1 || readCount % 2 == 1 )
           {
           mapKey = currentLine;
           }
        else
            {
            mapValue = currentLine;
            settingValuesMap.insert( mapKey, mapValue );
            }
        }

  QMapIterator <QString, QString>   mapIt( settingValuesMap );

  QString                           homeDir;
  QString                           subDir;
  QString                           settingsDir;
  QString                           applicationDir;
  QString                           projectName;
  QString                           projSettingsFile;

  /********************************************************
  *    iterates over the Map to construct the
  *    project settings absolute path and file name
  *    that is composed of:
  *      - homeDir
  *      - subDir
  *      - settingsDir
  *      - applicationDir
  *      - projectName
  *      - ".txt"
  ********************************************************/

  const QString homeDir_const           = "HOME_DIR";
  const QString subDir_const            = "SUB_DIR";
  const QString settingsDir_const       = "SETTINGS_DIR";
  const QString applicationDir_const    = "APPLICATION_DIR";
  const QString projectName_const       = "PROJECT_NAME";

  QString       dir1;
  QString       dir2;
  QString       dir3;

  while ( mapIt.hasNext() )
        {
        mapIt.next();

        if ( mapIt.key() == homeDir_const )
          homeDir = mapIt.value();

        if ( mapIt.key() == subDir_const )
          subDir = mapIt.value();

        if ( mapIt.key() == settingsDir_const )
          settingsDir = mapIt.value();

        if ( mapIt.key() == applicationDir_const )
          applicationDir = mapIt.value();

        if ( mapIt.key() == projectName_const )
          projectName = mapIt.value();

        dir1                = homeDir + subDir;
        dir2                = homeDir + subDir + settingsDir;
        dir3                = homeDir + subDir + settingsDir + applicationDir;

        projSettingsFile    = dir3 + projectName + ".txt";

        if ( ! QDir( dir1 ).exists() )
          QDir().mkdir( dir1 );

        if ( ! QDir( dir2 ).exists() )
          QDir().mkdir( dir2 );

        if ( ! QDir( dir3 ).exists() )
          QDir().mkdir( dir3 );
        }

  QFile file( projSettingsFile );

  if ( file.exists() )
     {
        qWarning() << "(" << proj << ")" << "File = " << projSettingsFile << " already exists. " <<  Qt::endl;
        qWarning() << "(" << proj << ")" << "It won't be created." <<  Qt::endl;

     return 0;
     }

  /********************************************************
  *     defines the project settings file
  ********************************************************/

  OpenFile_Lib  projSettingsFile_Out( projSettingsFile, proj, "WOTRTX" );
  QTextStream   myProjSettingsFile_Out( projSettingsFile_Out.getMp_File() );

  /********************************************************
  *     if file does not exist create it
  ********************************************************/

  if ( file.open( QIODevice::ReadWrite ) )
     {
     QTextStream stream( &file );
     stream << "# -- FILE CREATION ----------------" <<  Qt::endl;

     file.close();
     }
  else
      {
      qWarning()  << "(" << proj << ")"
                << Qt::endl
                << "The file = "
                << projSettingsFile
                << " could not be created."
                << Qt::endl
                << "Program will terminate." << Qt::endl;

      return EXIT_FAILURE;
      }

  /********************************************************
  *    iterates the Map again to write to ProjSettingsFile
  ********************************************************/

  mapIt.toFront();

  const QString spc_SemiColon_spc = " ; ";

  while ( mapIt.hasNext() )
        {
        mapIt.next();

        myProjSettingsFile_Out << mapIt.key() << spc_SemiColon_spc << mapIt.value() << Qt::endl;
        ++writeCount;
        }

  /********************************************************
  *             T O T A L S
  ********************************************************/

    qInfo() << "(" << proj << ")"
            << "Records read    =  "
            << readCount
            << Qt::endl;

    qInfo() << "(" << proj << ")"
            << "Records written =  "
            << writeCount
            << Qt::endl;

  }

/********************************************************
*          checkParameter function
********************************************************/
bool
checkParameter( const QString & fileName, const QString & proj )
  {
  QFileInfo fi_fileName( fileName );

  if ( !  fi_fileName.exists() )
     {
     qWarning()   << "(" << proj << ")"
                << Qt::endl
                << "The file = "
                << fileName
                << " does not exist."
                << Qt::endl
                << "Program will terminate." << Qt::endl;

     return ( false );
     }

  if ( !  fi_fileName.isFile() )
     {
     qWarning()   << "(" << proj << ")"
                << Qt::endl
                << "The file = "
                << fileName
                << " does not represent a file."
                << Qt::endl
                << "Program will terminate." << Qt::endl;

     return ( false );
     }

  if ( !  fi_fileName.isAbsolute() )
     {
     qWarning()   << "(" << proj << ")"
                << Qt::endl
                << "The file = "
                << fileName
                << " must be an absolute file path."
                << Qt::endl
                << "Program will terminate" << Qt::endl;

     return ( false );
     }

  return true;
  }
