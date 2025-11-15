#pragma once

#include <vsg/all.h>
#include <QString>
#include <QWindow>
#include <vsgQt/Window.h>
#include <vsgQt/Viewer.h>

class VsgWidget
{
private:
	vsgQt::Window* CreateWindow(vsg::ref_ptr<vsgQt::Viewer> viewer, vsg::ref_ptr<vsg::WindowTraits> traits, vsg::ref_ptr<vsg::Node> vsg_scene, QWindow* parent, const QString& title);
};

