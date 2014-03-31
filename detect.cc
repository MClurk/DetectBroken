/*
*  simulate the broken network detecting
*  attention: all the no.(of vertexes or of edges)start from 0
*/
#include <cstdlib>
#include <iostream>
#include <vector>
using namespace std;

struct Edge {
	bool edge_avaliable;
	int neibour;
	Edge(): edge_avaliable(true), neibour(0) {}
	Edge(int vertex): edge_avaliable(true), neibour(vertex) {}
};
struct Vertex {
	bool vertex_avaliable;
	int disconnect_vertex;  //for voting
	std::vector<int> new_neibours;  //new neibour's'
	std::vector<int> neibours;  //record of old neibours, for find new neibours
	std::vector<Edge> edges;
	Vertex():vertex_avaliable(true), disconnect_vertex(-1) {}  //-1 means none
};
bool constructTheGraph(const int number_of_centervertex,
					   const int number_of_subvertex,
					   vector<Vertex> &graph) {
	if(number_of_centervertex <= 0 || number_of_subvertex <= 0) {
		cout << "the parameter is wrong!" << endl;
		return false;
	}
	graph.assign(number_of_centervertex + number_of_subvertex * number_of_centervertex, Vertex());

	  //center vertex initialize
	int  sub_no = number_of_centervertex;  //no. of sub-vertexes
	for (int i = 0; i < number_of_centervertex; ++i) {
		Vertex &this_vertex = graph[i];
		this_vertex.neibours.assign(number_of_centervertex - 1 + number_of_subvertex, 0);
		this_vertex.edges.assign(number_of_centervertex - 1 + number_of_subvertex, Edge());
		int before_or_after = 0;
		for (int j = 0; j < number_of_centervertex - 1; ++j) {
			if (j == i)
				before_or_after = 1;
			this_vertex.edges[j].neibour = j + before_or_after;
			this_vertex.neibours[j] = j + before_or_after;
		}
		for(int j = number_of_centervertex - 1; j < number_of_centervertex - 1 + number_of_subvertex; ++j) {
			this_vertex.edges[j].neibour = sub_no;
			this_vertex.neibours[j] = sub_no;
			sub_no ++;
		}
	}
	  //sub vertex initialize
	int no = number_of_subvertex * number_of_centervertex;
	int i = number_of_centervertex;
	int center_no = 0;
	while(no--) {
		Vertex &this_vertex = graph[i];
		this_vertex.edges.assign(1, Edge(center_no));
		i ++;
		if (0 == (i - number_of_centervertex) % number_of_subvertex)
			center_no ++;
	}
	return true;
}

bool moveVertex(vector<Vertex> &graph,
				const int number_of_centervertex,
				const int number_of_subvertex,
				const int moving_vertex,
				const int want_move_to_vertex) {
	if (graph.empty())
		return false;
	int number_of_vertex = graph.size();
	if (moving_vertex >= number_of_vertex || moving_vertex < number_of_centervertex ||
			want_move_to_vertex >= number_of_centervertex || want_move_to_vertex < 0) {
		cout << "wrong vertex!" << endl;
		return false;
	}

	int old_attach_vertex = graph[moving_vertex].edges[0].neibour;
	Vertex &this_vertex = graph[old_attach_vertex];
	vector<Edge>::iterator iter = this_vertex.edges.begin();
	while(iter != this_vertex.edges.end()) {
		if (moving_vertex == (*iter).neibour)
			break;
		iter ++;
	}
	if (iter == this_vertex.edges.end()) {
		cout << "parameters wrong" << endl;
		return false;
	}
	(*iter).edge_avaliable = false;  //don't erase it!
	graph[want_move_to_vertex].edges.push_back(Edge(moving_vertex));
	graph[moving_vertex].edges[0].neibour = want_move_to_vertex;
	cout << "vertex has been moved!" << endl;
	return true;
}

bool breakTheNet(vector<Vertex> &graph, const int number) {  //break vertex
	if ( graph.empty() )
		return false;
	if ( number > (int)graph.size() - 1 || number < 0) {
		cout << "the parameter is out of range!" << endl;
		return false;
	}
	graph[number].vertex_avaliable = false;
	return true;
}

bool breakTheNet(vector<Vertex> &graph,
				 const int number_of_centervertex,
				 const int number_of_subvertex,
				 const int left_number,
				 const int right_number) {  //break edge
	if ( graph.empty() )
		return false;
	if ( left_number > (int)graph.size() - 1 || left_number < 0) {
		cout << "the parameter is out of range!" << endl;
		return false;
	}
	if ( right_number > (int)graph.size() - 1 || right_number < 0) {
		cout << "the parameter is out of range!" << endl;
		return false;
	}
	if ( left_number == right_number ) {
		cout << "wrong parameters!" << endl;
		return false;
	}

	int left_vertex = min(left_number, right_number);
	int right_vertex = max(left_number, right_number);

	if (left_vertex >= number_of_centervertex) {
		cout <<"wrong parameters!" << endl;
		return false;
	}  //vertexes of center will never move

	if (right_vertex < number_of_centervertex) {
		graph[left_vertex].edges[right_vertex - 1].edge_avaliable = false;
		graph[right_vertex].edges[left_vertex].edge_avaliable = false;
		return true;
	}  //vertexes of center will never move
	else {
		vector<Edge>::iterator iter = graph[left_vertex].edges.begin();
		while(iter != graph[left_vertex].edges.end()) {
			if (right_vertex == (*iter).neibour)
				break;
			iter ++;
		}
		if(iter == graph[left_vertex].edges.end()) {
			cout << "wrong parameters!" << endl;
			return false;
		}  //the edge is not exist
		(*iter).edge_avaliable = false;
		graph[right_vertex].edges[0].edge_avaliable = false;
	}
	return true;
}

void detectTheBreak(vector<Vertex>& graph,
		            const int number_of_centervertex,
		            const int number_of_subvertex) {
	if( graph.empty() )
		return;
	for (int i = 0; i < number_of_centervertex; ++i) {  //only the center vertexes have the right of vote
		if (graph[i].vertex_avaliable == false)
			continue;
		Vertex &this_vertex = graph[i];
		vector<Edge>::iterator iter = this_vertex.edges.begin();
		while(iter != this_vertex.edges.end()) {
			if ((*iter).edge_avaliable == false || graph[(*iter).neibour].vertex_avaliable == false) {
				cout << "vertex " << i << " to vertex " << (*iter).neibour << " is time out!" << endl;
				this_vertex.disconnect_vertex = (*iter).neibour;
			}  //time out
			else {
				cout << "vertex " << i << " to vertex " << (*iter).neibour << " is connected!" << endl;
				  //to judge if it's a new comer
				vector<int>::iterator nei_iter = this_vertex.neibours.begin();
				while(nei_iter != this_vertex.neibours.end()) {
					if (*nei_iter == (*iter).neibour)
						break;
					++ nei_iter;
				}
				if(nei_iter == this_vertex.neibours.end()) {
					this_vertex.new_neibours.push_back((*iter).neibour);
					  //accept it
					this_vertex.neibours.push_back((*iter).neibour);
				}
			}  //connected
			++ iter;
		}
	}
}

void voteBroken(const vector<Vertex> &graph, const int number_of_centervertex, vector<int> &vote) {
	if ( graph.empty() )
		return;

	int number_of_vertex = graph.size();
	vote.assign(number_of_vertex, 0);
	for (int i = 0; i < number_of_centervertex; ++i) {
		if(graph[i].vertex_avaliable == false)
			continue;
		if (graph[i].disconnect_vertex != -1)
			vote[graph[i].disconnect_vertex] += 1;
	}
}

bool isMovedOrNot(vector<int> &report_new_comer, const int i) {
	if (report_new_comer.empty())
		return false;  //no new comer
	vector<int>::const_iterator iter = report_new_comer.begin();
	while(iter != report_new_comer.end()) {
		if(i == *iter)
			break;
		++ iter;
	}
	if(iter == report_new_comer.end())
		return false;
	else
		return true;
}

int main(int argc, char** argv) {
	  //3 parameters: number of center vertex/ number of sub vertex/ broken vertex number
	  //4 parameters: number of center vertex/ number of sub vertex/ two vertex numbers of the broken edge
	  //4 parameters: number of center vertex/ number of sub vertex/ sub vertex want to move/ center vertex move to
	if ( argc < 4 ) {  //=4 or 5
		cout << "parameters wrong!" << endl;
		return -1;
	}
	  //construct the net
	vector<Vertex> graph;
	int number_of_centervertex = atoi(argv[1]);
	int number_of_subvertex = atoi(argv[2]);
	if ( !constructTheGraph(number_of_centervertex, number_of_subvertex, graph) )
		return -1;

	  //break the net
	enum Type {BROKEN,MOVED};
	Type net_type = MOVED;
	if (BROKEN == net_type) {
		if ( argc == 4 ) {
			if ( !breakTheNet( graph, atoi(argv[3]) ) )
				return -1;
		}
		else if ( argc == 5 )
			if ( !breakTheNet( graph, number_of_centervertex, number_of_subvertex, atoi(argv[3]), atoi(argv[4]) ) )
				return -1;
	}  //net_type == BROKEN
	else {
		if ( argc == 5 ) {
			if ( !moveVertex(graph, number_of_centervertex, number_of_subvertex, atoi(argv[3]), atoi(argv[4])))
					return -1;
		}
		else {
			cout << "wrong parameters!" <<endl;
			return -1;
		}
	}  //net_type == MOVED

	detectTheBreak(graph, number_of_centervertex, number_of_subvertex);

	vector<int> vote;
	voteBroken(graph, number_of_centervertex, vote);

	  //report new comers
	int number_of_vertex = graph.size();
	vector<int> report_new_comer;
	for (int v = 0; v < number_of_centervertex; ++v) {
		Vertex &this_vertex = graph[v];
		for (int n = 0; n < (int)this_vertex.new_neibours.size(); ++n) {
			report_new_comer.push_back(this_vertex.new_neibours[n]);
		}
	}
	  //check the vote
	for (int i = 0; i < number_of_vertex; ++i) {
		if (vote[i] > 1) {
			cout << "Now detected vertex " << i << " has broken!" << endl;
			return true;
		}
		else if (vote[i] == 1) {
			if (i < number_of_centervertex)
				cout << "Now detected edge of vertex " << i << " to vertex "
				<< graph[i].disconnect_vertex << " has broken!" << endl;
			else {
				if (isMovedOrNot(report_new_comer, i)) {
					cout << "vertex " << i << " is moved!" << endl;
				}
				else
					cout << "vertex " << i << " 's edge or node is broken!" << endl;
			}
		}
	}
}
