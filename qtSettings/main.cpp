#include "qtSettings_Proj.h"

int
main( int argc, char *argv[] )
  {
  QCoreApplication  a( argc, argv );
  QCoreApplication::setApplicationName( "qtSettings" );
  QString           appName = QCoreApplication::applicationName();

  qInfo() << "Application Name = " << appName << Qt::endl;

  QString           Operating_System;

  // Compile time decisions
#if defined( Q_OS_LINUX )
  Operating_System  = "LINUX";
#elif defined( Q_OS_WIN )
  Operating_System  = "WINDOWS";
#endif

  QSettings         settings1( QSettings::SystemScope );
  QSettings         settings2( "/home/alfredo/.PersonalApps/GeigerAlfredo/QSettingsExample.ini", QSettings::IniFormat );

//  QSettings         settings3( QSettings:: NativeFormat, QSettings::UserScope, "GeigerAlfredo", "QSettings Example Application" );
//  QSettings         settings4( "GeigerAlfredo", "QSettings Example Application" );

  // General Data
  settings2.setValue(   "home_dir",         "/home/alfredo/" );
  settings2.setValue(   "config_dir",       ".config/" );
  settings2.setValue(   "organization",     "GeigerAlfredo" );
  settings2.setValue(   "division",         "division" );
  settings2.setValue(   "application_name", "SelectBooks" );
  settings2.setValue(   "project_name",     "removePlural" );
  settings2.setValue(   "operating_system", Operating_System );

  settings2.setValue(   "author",           "Geiger, Alfredo" );

  // Grouped data
  settings2.setValue(   "Lang/programming", "C++" );
  settings2.setValue(   "Lang/framework",   "Qt Framework" );

    qInfo() << "Author : "
            << settings2.value( "author" ).toByteArray().constData()
            << Qt::endl;

    qInfo() << "Web : "
            << settings2.value( "web" ).toByteArray().constData()
            << Qt::endl;

    qInfo() << "Programming Lang : "
            << settings2.value( "Lang/programming" ).toByteArray().constData()
            << Qt::endl;

    qInfo() << "Framework : "
            << settings2.value( "Lang/framework" ).toByteArray().constData()
            << Qt::endl;

    qInfo() << "Experience : "
            << settings2.value( "Lang/experience" ).toByteArray().constData() << " years"
            << Qt::endl;

    qInfo() << Qt::endl << "------- ALL KEYS --------" << Qt::endl;

  QStringList keys = settings2.allKeys();

  for ( int i = 0; i < keys.size(); i++ )
      {
      qInfo()   << "Key : "
                << i
                << " / Value : "
                << keys.at( i ).toUtf8().constData()
                << Qt::endl;

      }

  qInfo() << Qt::endl << "------- GROUPS and SUBKEYS --------" << Qt::endl;

  QStringList groups = settings2.childGroups();

  for ( int i = 0; i < groups.size(); i++ )
      {
      qInfo()   << "Group : "
                << i << " = : "
                << groups.at( i ).toUtf8().constData()
                << Qt::endl;

      settings2.beginGroup( groups.at( i ).toUtf8().constData() );

      QStringList subKeyList = settings2.childKeys();

      for ( int j = 0; j < subKeyList.size(); j++ )
          {
          qInfo()   << "--SubKey : "
                    << j << " = : "
                    << subKeyList.at( j ).toUtf8().constData()
                    << Qt::endl;

          }

      settings2.endGroup();
      }

  return a.exec();
  }
