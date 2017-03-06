
#include "QmitkRegisterClasses.h"
#include "QmitkRenderWindow.h"

#include <mitkStandaloneDataStorage.h>

#include <itksys/SystemTools.hxx>
#include <QApplication>

#include <mitkIOUtil.h>

//##Documentation
//## @brief Load image (nrrd format) and display it in a 2D view
int main(int argc, char* argv[])
{
  QApplication qtapplication( argc, argv );

  if (argc < 2)
  {
    fprintf( stderr, "Usage:   %s [filename] \n\n", itksys::SystemTools::GetFilenameName(argv[0]).c_str() );
    return 1;
  }

  // Register Qmitk-dependent global instances
  QmitkRegisterClasses();

  //*************************************************************************
  // Part I: Basic initialization
  //*************************************************************************

  // Create a DataStorage
  // The DataStorage manages all data objects. It is used by the
  // rendering mechanism to render all data objects
  // We use the standard implementation mitk::StandaloneDataStorage.
  mitk::StandaloneDataStorage::Pointer datastorage = mitk::StandaloneDataStorage::New();


  //*************************************************************************
  // Part II: Create some data by reading files
  //*************************************************************************


  int i;
  for (i = 1; i< argc; ++i)
  {
	  //For testing
	  if (strcmp(argv[i], "-testing") == 0)
	    continue;
	    
  //*************************************************************************
  //Part III: Put the data into the dataStorage (add the node to the DataStorage)
  // Load datanode (eg. many image formats, surface formats, etc.)
      mitk::IOUtil::Load(argv[i],*datastorage);
  }

  //*************************************************************************
  // Part IV: Create window and pass the datastorage to it
  //*************************************************************************

  // Create a RenderWindow
  QmitkRenderWindow renderWindow;

  // Tell the RenderWindow which (part of) the datastorage to render
  renderWindow.GetRenderer()->SetDataStorage(datastorage);

  // Initialize the RenderWindow
  mitk::TimeGeometry::Pointer geo = datastorage->ComputeBoundingGeometry3D(datastorage->GetAll());
  mitk::RenderingManager::GetInstance()->InitializeViews( geo );
  //mitk::RenderingManager::GetInstance()->InitializeViews();

  // Select a slice
  mitk::SliceNavigationController::Pointer sliceNaviController = renderWindow.GetSliceNavigationController();
  if (sliceNaviController)
    sliceNaviController->GetSlice()->SetPos( 2 );

  //*************************************************************************
  // Part V: Qt-specific initialization
  //*************************************************************************
  renderWindow.show();
  renderWindow.resize( 256, 256 );

  qtapplication.exec();


  // cleanup: Remove References to DataStorage. This will delete the object
  datastorage = NULL;
}

