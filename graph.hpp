#include<iostream>
#include<type_traits>
#include<vector>
#include<list>
#include<queue>
#include<unordered_map>
#include<unordered_set>
#include<map>
#include <boost/functional/hash.hpp>
namespace graph{
template<typename E, typename V>
struct Graph{
  std::unordered_map<E, std::vector<V>> edges;

  std::vector<V> neighbours(const V& v) const{
    return neighbours_impl(v);
  }

  void addEdge(const V& v, const V& u){
    if(edges.count(v)){
      edges.at(v).push_back(u);
    } else {
      edges.emplace(v, {u});
    }
  }

  virtual std::vector<V> neighbours_impl(const V& v) const {
    return edges.at(v);
  }

  using vertex_t = V;
  using edge_t = E;
};

template<typename E, typename V, typename S>
struct ImplGraph : public Graph<E, V> {

  ImplGraph(const S& s) : s(s) {}

  std::vector<V> neighbours_impl(const V& v) const override {
    auto newStates = std::move(s(v));
    return newStates;
  }

  S s;
};

template<typename G, typename V>
auto bfs(const G& g, const typename G::vertex_t& start, const typename G::vertex_t& goal, const V& visitor){

  using vertex_t = typename G::vertex_t;
  std::unordered_set<vertex_t> visited;
  std::queue<vertex_t> q;
  q.push(start);
  visited.emplace(start);

  std::unordered_map<vertex_t, vertex_t> path;
  path[start] = start;

  while(!q.empty()){
    auto v = std::move(q.front());
    q.pop();
    visitor(v);
    if(v == goal){
      return path;
    }
    for(const auto& n : g.neighbours(v)){
      if(!visited.count(n)){
        q.push(n);
        visited.emplace(n);
        path[n] = v;
      }
    }
  }
  return path;
}

template<typename G, typename V, typename C, typename H>
auto astar(const G& g, const typename G::vertex_t& start,
    const typename G::vertex_t& goal, const V& visitor,
    const C& costFunction, const H& heuristic){

  using vertex_t = typename G::vertex_t;
  using cost_t = typename std::result_of<C(vertex_t,vertex_t)>::type;
  using heuristic_t = cost_t;//std::result_of<H(vertex_t,vertex_t)>::type;
  using pair_t = std::pair<heuristic_t, vertex_t>;
  std::priority_queue<pair_t, std::vector<pair_t>, std::greater<pair_t>> q;
  q.push({heuristic_t(), start});

  std::unordered_map<vertex_t, vertex_t> path;
  std::unordered_map<vertex_t, cost_t> cost;
  path[start] = start;
  cost[start] = cost_t();

  while(!q.empty()){
    auto [p, v] = std::move(q.top());
    q.pop();
    visitor(v);
    if(v == goal){
      return path;
    }
    for(const auto& n : g.neighbours(v)){
      auto newcost = cost.at(v) + costFunction(v, n);
      if(!cost.count(n) || newcost < cost.at(n)){
        cost[n] = newcost;
        auto h = heuristic(n, goal);
        q.push({newcost + h, n});
        path[n] = v;
      }
    }
  }
  return path;
}
}
