namespace Dictionary.Models;

public class Trie {
    private class Node
    {
        public Node[] Children { get; set; } = new Node[26];
        public int Exist { get; set; }
        public int Count { get; set; }
        public Node() {}
    }
    private readonly Node root;
    public Trie() {
        root = new Node();
    }
    public void AddString(string s)
    {
        var currentNode = root;
        foreach (char c in s) {
            int index = c - 'a';
            if (currentNode.Children[index] == null)
                currentNode.Children[index] = new Node();
            
            currentNode = currentNode.Children[index];
            currentNode.Count++;
        }
        currentNode.Exist++;
    }

    public bool FindString(string s)
    {
        var currentNode = root;
        foreach (char c in s)
        {
            int index = c - 'a';
            if (currentNode.Children[index] == null)
                return false;
            
            currentNode = currentNode.Children[index];
        }
        return currentNode.Exist != 0;
    }
}