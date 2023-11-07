using System;
using Dictionary.Models;

namespace Dictionary.Services;

public interface IAVLTreeService {
    public TNode? Insert(TNode? node, Word word);
    public Word? LookUp(string? key);
    public void InOrder(TNode? root);
    public TNode GetSample();
}

public class AVLTreeService : IAVLTreeService {
    private TNode sampleRoot;
    public AVLTreeService() {
        sampleRoot = GetSample();
    }

    public void InOrder(TNode? root) {
        if (root != null) {
            InOrder(root.Left);
            Console.Write(root.Word.KeyWord + " ");
            InOrder(root.Right);
        }
    }

    public TNode? Insert(TNode? node, Word word) {
        if (String.IsNullOrEmpty(word.KeyWord))
            return node;

        if (node == null)
            return new TNode(word);

        if (compareString(word.KeyWord, node.Word.KeyWord) < 0) 
            node.Left = Insert(node.Left, word);
        else if (compareString(word.KeyWord, node.Word.KeyWord) > 0)
            node.Right = Insert(node.Right, word);
        else
            return node;

        node.Height = 1 + Max(Height(node.Left), Height(node.Right));

        int balance = GetBalanceFactor(node);

        if (balance > 1 && compareString(word.KeyWord, node.Left.Word.KeyWord) < 0)
            return RightRotate(node);
        
        if (balance < -1 && compareString(word.KeyWord, node.Right.Word.KeyWord) > 0)
            return LeftRotate(node);
        
        if (balance > 1 && compareString(word.KeyWord, node.Left.Word.KeyWord) > 0) {
            node.Left = LeftRotate(node.Left);
            return RightRotate(node);
        } 
        
        if (balance < -1 && compareString(word.KeyWord, node.Right.Word.KeyWord) < 0) {
            node.Right = RightRotate(node.Right);
            return LeftRotate(node);
        }

        return node;
    }

    public TNode GetSample() {
        var words = new List<Word> {
            new() { KeyWord = "Abundant", PartOfSpeech = "adj", Meaning = "Available in large quantities; plentiful"},
            new() { KeyWord = "Compose", PartOfSpeech = "v", Meaning = "Write or create; constitute or make up"},
            new() { KeyWord = "Distinguish", PartOfSpeech = "v", Meaning = "Perceive or point out a difference"},
            new() { KeyWord = "Gauge", PartOfSpeech = "v", Meaning = "Estimate or determine the magnitude, amount, or volume of"},
            new() { KeyWord = "Intrepid", PartOfSpeech = "adj", Meaning = "Fearless; adventurous"},
            new() { KeyWord = "Orient", PartOfSpeech = "v", Meaning = "Align or position (something) relative to the points of a compass or other specified positions"},
            new() { KeyWord = "Residual", PartOfSpeech = "adj", Meaning = "Remaining after the greater part or quantity has gone"},
            new() { KeyWord = "Vindicate", PartOfSpeech = "v", Meaning = "Clear of blame or suspicion"},
            new() { KeyWord = "Aloof", PartOfSpeech = "adj", Meaning = "Not friendly or forthcoming; cool and distant"},
            new() { KeyWord = "Consecutive", PartOfSpeech = "adj", Meaning = "Following continuously, an unbroken or logical sequence"},
            new() { KeyWord = "Emerge", PartOfSpeech = "v", Meaning = "Move out of or away from something and come into view"},
            new() { KeyWord = "Hackneyed", PartOfSpeech = "adj", Meaning = "Lacking significance through having been overused"},
            new() { KeyWord = "Jubilation", PartOfSpeech = "n", Meaning = "A feeling of great happiness and triumph"}
        };

        TNode? root = null;

        foreach (Word word in words) {
            root = Insert(root, word);
        }

        return root!;
    }

    public Word? LookUp(string? searchStr) {
        if (String.IsNullOrEmpty(searchStr))
            return null;
        return Search(sampleRoot, searchStr);
    }
    public Word? Search(TNode? root, string searchStr) {
        if (root == null)
            return null;
        if (compareString(root.Word.KeyWord!, searchStr) < 0)
            return Search(root.Right, searchStr);
        else if (compareString(root.Word.KeyWord!, searchStr) > 0)
            return Search(root.Left, searchStr);
        else
            return root.Word;
    }

    private int Height(TNode? node) {
        if (node == null)
            return 0;
        return node.Height;
    }
    private int Max(int a, int b) {
        return (a > b) ? a : b;
    }

    private void UpdateHeight(TNode root) {
        root.Height = Max(Height(root.Left), Height(root.Right)) + 1;
    }

    private TNode RightRotate(TNode root) {
        TNode x = root.Left!;
        TNode? T2 = x.Right;

        x.Right = root;
        root.Left = T2;

        UpdateHeight(root);
        UpdateHeight(x);

        return x;
    }

    private TNode LeftRotate(TNode root) {
        TNode x = root.Right!;
        TNode? T2 = x.Left;

        x.Left = root;
        root.Right = T2;

        UpdateHeight(root);
        UpdateHeight(x);

        return x;
    }

    private int GetBalanceFactor(TNode root) {
        if (root == null)
            return 0;
        
        return Height(root.Left) - Height(root.Right);
    }

    private int compareString(string str1, string str2) {
        return String.Compare(str1, str2, comparisonType: StringComparison.OrdinalIgnoreCase);
    }
}