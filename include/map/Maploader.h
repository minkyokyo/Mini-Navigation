namespace map
{
    struct Vertex
    {
        float x, y; // NDC
        float r, g, b, a;
        float type; // -1 road, 0 red, 1 blue, 2 green(circle)
    };
}
