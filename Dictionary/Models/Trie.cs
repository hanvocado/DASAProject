using Dictionary.Constants;

namespace Dictionary.Models;

public class Trie {
    public class Node
    {
        public Node?[] Children { get; set; } = new Node[Constant.ALPHABET_SIZE];
        public bool isEndOfWord;
        public Node() {
            isEndOfWord = false;
            for (int i = 0; i < Constant.ALPHABET_SIZE; i++)
                Children[i] = null;
        }
    }
    private readonly Node root;
    public Trie() {
        root = new Node();
    }

    // If not present, inserts key into trie
    // If the key is prefix of trie node, 
    // just marks leaf node
    public void Insert(string key)
    {
        Node pCrawl = root;
        foreach (char c in key) {
            int index = c - 'a';
            if (c == '-')
                index = 26;

            if (pCrawl.Children[index] == null)
                pCrawl.Children[index] = new Node();
            
            pCrawl = pCrawl.Children[index]!;
        }
        pCrawl.isEndOfWord = true;
    }

    public bool Found(string key)
    {
        var pCrawl = root;
        foreach (char c in key)
        {
            int index = c - 'a';
            if (c == '-')
                index = 26;
            if (pCrawl!.Children[index] == null)
                return false;
            
            pCrawl = pCrawl.Children[index];
        }
        return pCrawl!.isEndOfWord;
    }

    public bool IsLastNode(Node node) {
        for (int i = 0; i < Constant.ALPHABET_SIZE; i++) {
            if (node.Children[i] != null)
                return false;
        }
        return true;
    }

    // Recursive function to print auto-suggestions for given
    // node.
    private void SuggestRec(List<string> suggestions, Node? node, string currentPrefix) {
        if (node == null) return;
        if (node.isEndOfWord) {
            suggestions.Add(currentPrefix);
        }
        for (int i = 0; i < Constant.ALPHABET_SIZE; i++) {
            if (node.Children[i] != null) {
                char c = (char)(i + 'a');
                SuggestRec(suggestions, node.Children[i], currentPrefix + c);
            }   
        }
    }

    public List<string> Suggest(string query) {
        List<string> suggestions = new();
        Node pCrawl = root;
        foreach (char c in query) {
            int index = c - 'a';
            if (c == '-')
                index = 26;
            // no string in the Trie has this prefix
            if (pCrawl.Children[index] == null)
                return suggestions;
            pCrawl = pCrawl.Children[index]!;
        }
        // If prefix is present as a word, but
        // there is no subtree below the last
        // matching node.
        if (IsLastNode(pCrawl)) {
            suggestions.Add(query);
            return suggestions;
        }
        SuggestRec(suggestions, pCrawl, query);
        return suggestions;
    }
}