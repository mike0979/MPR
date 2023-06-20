/******************************************************************************
*  Copyright (C), 2017-2019,Advanced Technology Institute of Suzhou. 
*  THIS IS AN UNPUBLISHED WORK CONTAINING CONFIDENTIAL AND PROPRIETARY 
*   
*******************************************************************************
*  File Name: ResliceCursorCallback.h
*  Author: cg.zhang@atisz.ac.cn
*  Version: 1.0.0
*  Date: 2020-5-22
*  Description: brief
*  History: 
******************************************************************************/

#ifndef RESLICE_CURSOR_CALLBACK_H
#define RESLICE_CURSOR_CALLBACK_H

#include <vtkCommand.h>
#include "vtkResliceImageViewer.h"
#include "vtkInteractorStyleImage.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkResliceCursorWidget.h"
#include "vtkImageReslice.h"
#include "vtkResliceCursorLineRepresentation.h"
#include "vtkImagePlaneWidget.h"
#include "vtkPlaneSource.h"
#include "vtkResliceCursorActor.h"
#include "vtkRenderWindow.h"
#include "vtkResliceCursorPolyDataAlgorithm.h"
#include "vtkImageData.h"

// Define own interaction style

class vtkResliceCursorCallback : public vtkCommand
{
public:
	static vtkResliceCursorCallback *New()
	{
		return new vtkResliceCursorCallback;
	}

	virtual void Execute(vtkObject *caller, unsigned long ev, void *callData)
	{
		for (int i = 0; i < 3; i++)
		{
			this->RCW[i]->Render();
		}
	}

	vtkResliceCursorCallback() {}
	vtkResliceImageViewer *RCW[3];
};

#endif
