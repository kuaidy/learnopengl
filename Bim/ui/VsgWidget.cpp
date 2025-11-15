#include "VsgWidget.h"

vsgQt::Window* CreateWindow(vsg::ref_ptr<vsgQt::Viewer> viewer, vsg::ref_ptr<vsg::WindowTraits> traits, vsg::ref_ptr<vsg::Node> vsg_scene, QWindow* parent, const QString& title = {})
{
    auto window = new vsgQt::Window(viewer, traits, parent);

    window->setTitle(title);

    window->initializeWindow();

    // if this is the first window to be created, use its device for future window creation.
    if (!traits->device) traits->device = window->windowAdapter->getOrCreateDevice();

    // compute the bounds of the scene graph to help position camera
    vsg::ComputeBounds computeBounds;
    vsg_scene->accept(computeBounds);
    vsg::dvec3 centre = (computeBounds.bounds.min + computeBounds.bounds.max) * 0.5;
    double radius = vsg::length(computeBounds.bounds.max - computeBounds.bounds.min) * 0.6;
    double nearFarRatio = 0.001;

    uint32_t width = window->traits->width;
    uint32_t height = window->traits->height;

    vsg::ref_ptr<vsg::EllipsoidModel> ellipsoidModel(vsg_scene->getObject<vsg::EllipsoidModel>("EllipsoidModel"));
    vsg::ref_ptr<vsg::Camera> camera;
    {
        // set up the camera
        auto lookAt = vsg::LookAt::create(centre + vsg::dvec3(0.0, -radius * 3.5, 0.0), centre, vsg::dvec3(0.0, 0.0, 1.0));

        vsg::ref_ptr<vsg::ProjectionMatrix> perspective;
        if (ellipsoidModel)
        {
            perspective = vsg::EllipsoidPerspective::create(
                lookAt, ellipsoidModel, 30.0,
                static_cast<double>(width) /
                static_cast<double>(height),
                nearFarRatio, false);
        }
        else
        {
            perspective = vsg::Perspective::create(
                30.0,
                static_cast<double>(width) /
                static_cast<double>(height),
                nearFarRatio * radius, radius * 4.5);
        }

        camera = vsg::Camera::create(perspective, lookAt, vsg::ViewportState::create(VkExtent2D{ width, height }));
    }

    auto trackball = vsg::Trackball::create(camera, ellipsoidModel);
    trackball->addWindow(*window);

    viewer->addEventHandler(trackball);

    auto commandGraph = vsg::createCommandGraphForView(*window, camera, vsg_scene);

    viewer->addRecordAndSubmitTaskAndPresentation({ commandGraph });

    return window;
}
