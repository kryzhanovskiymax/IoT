#pragma once

#include <unordered_map>
#include <vector>
#include <cstdlib>
#include <ctime>

#include "svg/svg.hpp"

namespace graph {

namespace domain {

    struct Size {
        int height = 400;
        int width = 400;
        int padding = 15;
    };  

    struct GraphRepresentationConfig {
        int stroke_width = 2;
        int vertice_radius = 15;
        int mst_line_width = 3;
        std::vector<svg::Color> palette = {svg::Rgb(45, 45, 60)};
        std::vector<svg::Color> vertice_palette = {svg::Rgb(60, 80, 190)};
        std::vector<svg::Color> edge_palette = {svg::Rgb(200, 100, 100)};
        svg::Color mst_color = svg::Rgb(10, 150, 10);
    };

    struct RepresentationFlags {
        bool highlight_mst = false;
        bool heighlight_path = false;
    };

}

class GraphPainter {
public:

    GraphPainter() {};
    GraphPainter(domain::Size size, domain::GraphRepresentationConfig config, domain::RepresentationFlags flags):
                    image_size(size), representation_config(config), representation_flags(flags) {}

    void InitializeGraphPainter(Graph<std::string> graph_, Graph<std::string> mst_) {
        this->graph = graph_;
        this->mst = mst_;
    }

    void InitializeGraphPainter(Graph<std::string> graph_) {
        this->graph = graph_;
    }

    void RenderGraph(std::ostream& os) {
        auto positions = GeneratePositions(this->graph.vertices.size());

        int i = 0;
        for (const auto& v : this->graph.vertices) {
            vertices_to_positions[v] = positions[i];
            ++i;
        }

        svg::Document canvas;
        DrawGraphRepresentation(canvas);
        canvas.Render(os);
    }
    
    GraphPainter& SetStrokeWidth (int width) {
        representation_config.stroke_width = width;
        return *this;
    }

    GraphPainter& SetVerticeRadius (int radius) {
        representation_config.vertice_radius = radius;
        return *this;
    }

    GraphPainter& SetMSTLineWidth (int width) {
        representation_config.mst_line_width = width;
        return *this;
    }

    GraphPainter& SetVerticePalette (std::vector<svg::Color> colors) {
        representation_config.vertice_palette = colors;
        return *this;
    }

    GraphPainter& SetEdgePalette (std::vector<svg::Color> colors) {
        representation_config.edge_palette = colors;
        return *this;
    }

    GraphPainter& SetMSTEdgeColor (svg::Color color) {
        representation_config.mst_color = color;
        return *this;
    }

    GraphPainter& SetMST(Graph<std::string> mst_) {
        this->mst = mst_;
        return *this;
    }

    GraphPainter& SetGraph(Graph<std::string> graph_) {
        graph = graph_;
        return *this;
    } 

    GraphPainter& SetMSTHighlight(bool mst_highlight) {
        representation_flags.highlight_mst = mst_highlight;
        return *this;
    } 

    GraphPainter& SetSize(int width, int height, int padding) {
        image_size.width = width;
        image_size.height = height;
        image_size.padding = padding;
        return *this;
    }

    std::vector<svg::Point> GetPositions(int num) {
        return GeneratePositions(num);
    } 

private:
    domain::Size image_size;
    domain::GraphRepresentationConfig representation_config;
    domain::RepresentationFlags representation_flags;
    std::unordered_map<std::string, svg::Point> vertices_to_positions;
    Graph<std::string> graph;
    Graph<std::string> mst;

    std::vector<svg::Point> GeneratePositions(int num_of_positions) {
        srand(time(NULL));
        int gap_between_rows = representation_config.vertice_radius;
        int gap_between_vertices = representation_config.vertice_radius;

        int pixel_width = 2 * (representation_config.vertice_radius + gap_between_vertices);
        int pixel_height = 2 * (representation_config.vertice_radius + gap_between_rows);
        int elements_per_row = (image_size.width - image_size.padding * 2) / pixel_width;
        int elements_per_column = (image_size.height - image_size.padding * 2) / pixel_height;

        std::set<svg::Point> positions;

        while(positions.size() < num_of_positions) {
            svg::Point pos;
            pos.x = image_size.padding + (rand() % elements_per_row) * pixel_width 
                    + gap_between_rows + representation_config.vertice_radius;
            pos.y = image_size.padding + (rand() % elements_per_row) * pixel_height 
                    + gap_between_rows + representation_config.vertice_radius;

            positions.insert(pos);
        }

        return {positions.begin(), positions.end()};
    }

    void DrawGraphRepresentation(svg::Document& image) {
        DrawGraphEdges(image);
        DrawVertices(image);

        if (representation_flags.highlight_mst) {
            DrawMSTEdges(image);
        }

        DrawVerticesNames(image);
    }

    void DrawVertices(svg::Document& image) {
        for (const auto& [vertice, position] : vertices_to_positions) {
            svg::Circle vertice_mark;
            vertice_mark.SetCenter(position);
            vertice_mark.SetFillColor(representation_config.vertice_palette[0]);
            vertice_mark.SetStrokeColor(representation_config.vertice_palette[0]);
            vertice_mark.SetRadius(representation_config.vertice_radius);
            image.Add(vertice_mark);
        }
    }

    void DrawVerticesNames(svg::Document& image) {
        for (const auto& [vertice, position] : vertices_to_positions) {
            svg::Text vertice_name;
            svg::Text underlayer;

            svg::Point offset{-1, -1};
            vertice_name.SetPosition(position).SetOffset(offset);
            vertice_name.SetFontSize(10).SetFontFamily("Verdana").SetData(vertice).SetFillColor("balck");

            underlayer.SetPosition(position).SetOffset(offset).SetFontSize(10).SetFontFamily("Verdana").SetData(vertice).SetFillColor("balck");
            underlayer.SetStrokeColor("white").SetFillColor("white").SetStrokeWidth(12).SetStrokeLineJoin(svg::StrokeLineJoin::ROUND).SetStrokeLineCap(svg::StrokeLineCap::ROUND);

            image.Add(underlayer);
            image.Add(vertice_name);
        }
    }

    void DrawGraphEdges(svg::Document& image) {
        for (const auto& edge_ : graph.edges) {
            svg::Polyline edge;
            svg::Point first_point = vertices_to_positions.at(edge_.first);
            svg::Point second_point = vertices_to_positions.at(edge_.second);

            edge.AddPoint(first_point);
            edge.AddPoint(second_point);

            edge.SetFillColor(svg::NoneColor).SetStrokeColor(representation_config.edge_palette[0]);
            edge.SetStrokeLineCap(svg::StrokeLineCap::ROUND).SetStrokeLineJoin(svg::StrokeLineJoin::ROUND);

            edge.SetStrokeWidth(representation_config.stroke_width);

            image.Add(edge);
        }
    }

    void DrawMSTEdges(svg::Document& image) {
        for (const auto& edge_ : mst.edges) {
            svg::Polyline edge;
            svg::Point first_point = vertices_to_positions.at(edge_.first);
            svg::Point second_point = vertices_to_positions.at(edge_.second);

            edge.AddPoint(first_point);
            edge.AddPoint(second_point);

            edge.SetFillColor(svg::NoneColor).SetStrokeColor(representation_config.mst_color);
            edge.SetStrokeLineCap(svg::StrokeLineCap::ROUND).SetStrokeLineJoin(svg::StrokeLineJoin::ROUND);

            edge.SetStrokeWidth(representation_config.mst_line_width);

            image.Add(edge);
        }
    }
};

}