#include "resources/mesh.h"

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices)
    : Vertices(vertices), Indices(indices)
{
}
