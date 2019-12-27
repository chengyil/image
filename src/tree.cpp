using namespace std;
#include <vector>
#include <map>
#include<iostream>

class Node
{
  public:
    std::vector<Node> children;
    Node *right = NULL;
    int val;
};

void populateTest(Node &node){
  int count = 0;
  node.val = count++;
  for(int i = 0; i < 3; i++)
  {
    Node child;
    child.val = count++;
    node.children.push_back(child);
  }

  for (int i = 0; i < node.children.size(); i++)
  {
    for (int j = 0; j < 3; j++)
    {
      Node child;
      child.val = count;
      count++;
      node.children[i].children.push_back(child);
    }
  }
}

void buildRight(Node &node, int level, std::map<int,Node*> &m)
{
  cout << "building " << node.val << endl;
  if (m.find(level) != m.end())
  {
    m[level]->right = &node;
    m[level] = &node;
  }
  else
  {
    m[level] = &node;
  }

  for (int i = 0; i < node.children.size(); i++)
  {
    buildRight(node.children[i], level + 1, m);
  }
}

void walkTree(Node &node)
{
  cout << "Current val : " << node.val << endl;
  Node *current = node.right;
  while(current != NULL)
  {
    cout << "Current val : " << current->val << endl;
    current = current->right;
  }
  if(node.children.size() > 0)
  {
    walkTree(node.children[0]);
  }
}


int main(int argc, char** argv)
{
  std::map<int, Node*> m;
  Node root;
  populateTest(root);
  buildRight(root, 0, m);
  walkTree(root);
  return 0;
}
