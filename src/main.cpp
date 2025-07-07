#include <pybind11/pybind11.h>
#include <bits/stdc++.h>
using namespace std;
namespace py = pybind11;

template <typename DataType>
class Graph {
public:
    void add_edge(int from, int to, DataType data) {
        edge[from].emplace_back(to, data);
    }

    void remove_edge(int from, int to) {
        if (!edge.count(from)) return;
        auto& edges = edge[from];
        edges.erase(
            std::remove_if(edges.begin(), edges.end(),
                [to](const auto& pair) { return pair.first == to; }),
            edges.end());
    }

    void remove_vertex(int u) {
        edge.erase(u);
        for (auto& [from, edges] : edge) {
            edges.erase(
                std::remove_if(edges.begin(), edges.end(),
                    [u](const auto& pair) { return pair.first == u; }),
                edges.end());
        }
    }

    void clear_all() {
        edge.clear();
    }

    std::optional<DataType> get_edge(int from, int to) const {
        if (!edge.count(from)) return std::nullopt;
        for (const auto& c : edge.at(from)) {
            if (c.first == to) return c.second;
        }
        return std::nullopt;
    }

    bool has_vertex(int u) const {
        return edge.count(u) > 0;
    }

    template <typename CallbackType>
    void dfs(int start, CallbackType&& cb) {
        std::unordered_set<int> vis;
        _dfs(start, std::forward<CallbackType>(cb), vis);
    }

private:
    template <typename CallbackType>
    void _dfs(int u, CallbackType&& func, std::unordered_set<int>& vis) {
        if (!vis.insert(u).second) return;
        
        if (!edge.count(u)) return;
        
        for (const auto& c : edge.at(u)) {
            func(u, c.first, c.second);
            _dfs(c.first, std::forward<CallbackType>(func), vis);
        }
    }

    std::map<int, std::vector<std::pair<int, DataType>>> edge;
};
using PyGraph = Graph<py::object>;

PYBIND11_MODULE(graph, m) {
    m.doc() = "Graph data structure with Python objects as edge data";
    
    py::class_<PyGraph>(m, "Graph")
        .def(py::init<>())
        .def("add_edge", [](PyGraph &g, int from, int to, py::object data) {
            g.add_edge(from, to, data);
        }, py::arg("from"), py::arg("to"), py::arg("data"),
           "Add an edge from 'from' to 'to' with associated Python object")
        
        .def("remove_edge", &PyGraph::remove_edge, 
             py::arg("from"), py::arg("to"),
             "Remove the edge from 'from' to 'to'")
        
        .def("remove_vertex", &PyGraph::remove_vertex, 
             py::arg("u"),
             "Remove the vertex and all its associated edges")
        
        .def("clear_all", &PyGraph::clear_all,
             "Clear all vertices and edges")
        
        .def("get_edge", [](const PyGraph &g, int from, int to) -> py::object {
            auto opt = g.get_edge(from, to);
            return opt ? *opt : py::none();
        }, py::arg("from"), py::arg("to"),
           "Get the data associated with the edge from 'from' to 'to'")
        
        .def("has_vertex", &PyGraph::has_vertex, 
             py::arg("u"),
             "Check if the vertex exists in the graph")
        
        .def("dfs", [](PyGraph &g, int start, const py::function &cb) {
            g.dfs(start, [&cb](int from, int to, const py::object &data) {
                cb(from, to, data);
            });
        }, py::arg("start"), py::arg("callback"),
           "Perform depth-first search starting from 'start' vertex, calling callback for each edge");
}