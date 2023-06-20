/******************************************************************************
*  Copyright (C), 2017-2019,Advanced Technology Institute of Suzhou. 
*  THIS IS AN UNPUBLISHED WORK CONTAINING CONFIDENTIAL AND PROPRIETARY 
*   
*******************************************************************************
*  File Name: Widget.cpp
*  Author: cg.zhang@atisz.ac.cn
*  Version: 1.0.0
*  Date: 2020-3-19
*  Description: 三维重建
*  History: 
******************************************************************************/

#include "Rebuild3D.h"
#include "ResliceCursorCallback.h"
#include "vtkRenderer.h"
#include "QVTKOpenGLWidget.h"
#include "vtkOutlineFilter.h"
#include <vtkRenderWindow.h>
#include <vtkImageViewer2.h>
#include "vtkImageReslice.h"
#include "vtkImageData.h"
#include "vtkImageReader2.h"
#include "vtkAutoInit.h"
#include "vtkLookupTable.h"
#include "vtkImageMapToColors.h"
#include "vtkCellPicker.h"
#include "vtkProperty.h"
#include "vtkImageActor.h"

VTK_MODULE_INIT(vtkRenderingOpenGL2)
VTK_MODULE_INIT(vtkInteractionStyle)
VTK_MODULE_INIT(vtkRenderingFreeType)

//vtkStandardNewMacro(InteractorStyleImage);

#define VIEW_COUNT 3

Rebuild3D::Rebuild3D(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(onResliceMode()));
	connect(ui.pushButton_2, SIGNAL(clicked()), this, SLOT(onViewMode()));
	m_renderWindow[0] = ui.vtkSagittal->GetRenderWindow();
	m_renderWindow[1] = ui.vtkCoronal->GetRenderWindow(); 
	m_renderWindow[2] = ui.vtkHorizontal->GetRenderWindow();
	//m_renderWindow[3] = ui.widget_3->GetRenderWindow();
}

Rebuild3D::~Rebuild3D()
{

}

void Rebuild3D::Rebuild(vtkImageReader2* pReader)
{
	for (int i = 0; i < 3; ++i) 
	{
		m_resliceImageViewer[i] = vtkSmartPointer<vtkResliceImageViewer>::New();
		m_resliceImageViewer[i]->SetRenderWindow(m_renderWindow[i]);
		m_resliceImageViewer[i]->SetupInteractor(m_renderWindow[i]->GetInteractor());
		m_resliceImageViewer[i]->SetResliceCursor(m_resliceImageViewer[0]->GetResliceCursor());
	}

	vtkSmartPointer<vtkResliceCursorCallback> resliceCallback = vtkSmartPointer<vtkResliceCursorCallback>::New();
	for (int i = 0; i < 3; ++i) 
	{
		resliceCallback->RCW[i] = m_resliceImageViewer[i];
		m_resliceImageViewer[i]->GetResliceCursorWidget()->AddObserver(vtkResliceCursorWidget::ResliceAxesChangedEvent, resliceCallback);
		m_resliceImageViewer[i]->GetInteractorStyle()->AddObserver(vtkCommand::LeftButtonPressEvent, resliceCallback);
	}

	pReader->Update();
	int imageDims[3];
	pReader->GetOutput()->GetDimensions(imageDims);

	for (int i = 0; i < 3; ++i)
	{
		auto rep = vtkResliceCursorLineRepresentation::SafeDownCast(m_resliceImageViewer[i]->GetResliceCursorWidget()->GetRepresentation());
		rep->GetResliceCursorActor()->GetCursorAlgorithm()->SetReslicePlaneNormal(i);
		m_resliceImageViewer[i]->SetInputData(pReader->GetOutput());
		m_resliceImageViewer[i]->SetSliceOrientation(i);
		m_resliceImageViewer[i]->SetResliceModeToAxisAligned();
	}

	for (int i = 0; i < 3; ++i) 
	{
		m_resliceImageViewer[i]->GetRenderer()->ResetCamera();
		m_renderWindow[i]->Render();
	}
}

void Rebuild3D::onResliceMode()
{
	for (int i = 0; i < 3; ++i)
	{
		m_resliceImageViewer[i]->SetResliceModeToOblique();
		m_resliceImageViewer[i]->GetRenderer()->ResetCamera();
		m_resliceImageViewer[i]->Render();
	}
}

void Rebuild3D::onViewMode()
{
	for (int i = 0; i < 3; ++i)
	{
		m_resliceImageViewer[i]->SetResliceModeToAxisAligned();
		m_resliceImageViewer[i]->GetRenderer()->ResetCamera();
		m_resliceImageViewer[i]->Render();
	}
}
