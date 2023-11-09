public class Trie
{
    private class Node
    {
        public Node[] Children { get; private set; } = new Node[26];
        public int Exist { get; set; }
        public int Count { get; set; }

        public Node() { }
    }

    private readonly Node root;

    public Trie()
    {
        root = new Node();
    }

    public void AddString(string s)
    {
        var current = root;
        foreach (char c in s)
        {
            int index = c - 'a';
            if (current.Children[index] == null)
            {
                current.Children[index] = new Node();
            }

            current = current.Children[index];
            current.Count++;
        }

        current.Exist++;
    }

    public bool DeleteString(string s)
    {
        if (!FindString(s))
        {
            return false;
        }

        return DeleteStringRecursive(root, s, 0);
    }

    private bool DeleteStringRecursive(Node node, string s, int index)
    {
        if (index != s.Length)
        {
            int childIndex = s[index] - 'a';
            bool isChildDeleted = DeleteStringRecursive(node.Children[childIndex], s, index + 1);
            if (isChildDeleted)
            {
                node.Children[childIndex] = null;
            }
        }
        else
        {
            node.Exist--;
        }

        if (node != root)
        {
            node.Count--;
            if (node.Count == 0)
            {
                // Unlike the array implementation,
                // we can actually delete this node.
                return true;
            }
        }

        return false;
    }

    public bool FindString(string s)
    {
        var current = root;
        foreach (char c in s)
        {
            int index = c - 'a';
            if (current.Children[index] == null)
            {
                return false;
            }

            current = current.Children[index];
        }

        return current.Exist != 0;
    }
}