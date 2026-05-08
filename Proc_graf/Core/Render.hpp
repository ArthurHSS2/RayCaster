#ifndef RENDER_H
#define RENDER_H

#include "Camera.hpp"
#include "Scene.hpp"
#include <fstream>

class Render
{
    public:
        void renderize(const Camera& cam, const Scene& cena);
};

#endif