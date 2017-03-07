
#include "QmitkRegisterClasses.h"
#include "QmitkRenderWindow.h"
#include "QmitkSliceWidget.h"
#include "mitkNodePredicateDataType.h"
#include "mitkProperties.h"
#include "mitkRenderingManager.h"
#include "mitkStandaloneDataStorage.h"

#include <mitkIOUtil.h>
#include <QApplication>
#include <QHBoxLayout>
#include <itksys/SystemTools.hxx>
#include <mitkImage.h>


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
	  mitk::IOUtil::Load(argv[i], *datastorage);
  }
  //*************************************************************************
  //Part III: Put the data into the dataStorage (add the node to the DataStorage)
  // Load datanode (eg. many image formats, surface formats, etc.)
  /* This is to load data and show it in the 3d render  
   * mitk::StandaloneDataStorage::SetOfObjects::Pointer dataNodes = mitk::IOUtil::Load(argv[i], *datastorage);
      if (dataNodes->empty())
      {
        fprintf(stderr, "Could not open file %s \n\n", argv[i]);
        exit(2);
      }
    mitk::DataNode::Pointer node = dataNodes->at(0);*/
  
     
   
  //************************************************************************
  // Part IIIa: we want all the images to be volume rendered
  //***********************************************************
  
  //We first check that the data is an image
   /* mitk::Image::Pointer image = dynamic_cast<mitk::Image *>(node->GetData());
  
	if (image.IsNotNull())
	{
	  
	  // Set the property "volumerendering" to the Boolean value "true"
	  node->SetProperty("volumerendering", mitk::BoolProperty::New(true));
	  // Create a transfer function to assign optical properties to grey-values of the data
	  mitk::TransferFunction::Pointer tf = mitk::TransferFunction::New();
	  tf->InitializeByMitkImage(image);
	   
	  // Set the color transfer function AddRGBPoint(double x, double r, double g, double b)
	  tf->GetColorTransferFunction()->AddRGBPoint(tf->GetColorTransferFunction()->GetRange()[0], 1.0, 0.0, 0.0);
	  tf->GetColorTransferFunction()->AddRGBPoint(tf->GetColorTransferFunction()->GetRange()[1], 1.0, 1.0, 0.0);
	  
	  // Set the piecewise opacity transfer function AddPoint(double x, double y)
	  tf->GetScalarOpacityFunction()->AddPoint(0, 0);
	  tf->GetScalarOpacityFunction()->AddPoint(tf->GetColorTransferFunction()->GetRange()[1], 1);
	  
	  node->SetProperty("TransferFunction", mitk::TransferFunctionProperty::New(tf.GetPointer()));
	}*/
  
  
  //*************************************************************************
  // Part IV: Create window and pass the datastorage (tree) to it
  //*************************************************************************
  
  //Create toplevel widget with horizontal layout
  QWidget toplevelWidget;
  QHBoxLayout layout;
  layout.setSpacing(2);
  layout.setMargin(0);
  toplevelWidget.setLayout(&layout);
  
  //**********************************
  // Part IVa: 3D view
  //*******************************************
  
  // Create a RenderWindow
  QmitkRenderWindow renderWindow(&toplevelWidget);
  layout.addWidget(&renderWindow);
  
  // Tell the RenderWindow which (part of) the datastorage to render
  renderWindow.GetRenderer()->SetDataStorage(datastorage);
  
  //use it as a 3D view
  renderWindow.GetRenderer()->SetMapperID(mitk::BaseRenderer::Standard3D);
  
  //reposition the camera to include all visible actors
  renderWindow.GetRenderer()->GetVtkRenderer()->ResetCamera();
  
  
  //Create a 2D view for slicing axially (IVb)
  QmitkSliceWidget view2(&toplevelWidget);
  layout.addWidget(&view2);
  view2.SetLevelWindowEnabled(true);
  view2.SetDataStorage(datastorage);
  
  //Get the image from the data storage
  mitk::DataStorage::SetOfObjects::ConstPointer rs = datastorage->GetSubset(mitk::TNodePredicateDataType<mitk::Image>::New());
  
  view2.SetData(rs->Begin(), mitk::SliceNavigationController::Axial);
  
  // We want to see the position of the slice in 2D and the
  // slice itself in 3D: so we add it to the datastorage.
  
  
  //Create a 2D view for slicing sagitally (IVc)
  QmitkSliceWidget view3(&toplevelWidget);
  layout.addWidget(&view3);
  view3.SetDataStorage(datastorage);
  
  view3.SetData(rs->Begin(), mitk::SliceNavigationController::Sagittal);
  
  //return qtapplication.exec();
  
  datastorage->Add(view2.GetRenderer()->GetCurrentWorldPlaneGeometryNode());
  datastorage->Add(view3.GetRenderer()->GetCurrentWorldPlaneGeometryNode());
  
  //return qtapplication.exec()
  
  
  //**************************
  //Part IIIb (for step 3)
  //***************************
  /*
  // Use it as a 3D view!
  renderWindow.GetRenderer()->SetMapperID(mitk::BaseRenderer::Standard3D);
  // Reposition the camera to include all visible actors
  renderWindow.GetRenderer()->GetVtkRenderer()->ResetCamera();
  
  // Initialize the RenderWindow
  mitk::TimeGeometry::Pointer geo = datastorage->ComputeBoundingGeometry3D(datastorage->GetAll());
  mitk::RenderingManager::GetInstance()->InitializeViews( geo );
  //mitk::RenderingManager::GetInstance()->InitializeViews();

  // Select a slice
  //mitk::SliceNavigationController::Pointer sliceNaviController = renderWindow.GetSliceNavigationController();
 // if (sliceNaviController)
    //sliceNaviController->GetSlice()->SetPos( 2 );
   */
  //*************************************************************************
  // Part V: Qt-specific initialization
  //*************************************************************************
  //renderWindow.show();
  //enderWindow.resize( 256, 256 );

  toplevelWidget.show();
  
  //

  qtapplication.exec();
  // cleanup: Remove References to DataStorage. This will delete the object
  datastorage = NULL;
}

