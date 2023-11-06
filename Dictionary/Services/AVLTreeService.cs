using System;
using Dictionary.Models;

namespace Dictionary.Services;

public interface IAVLTreeService {
    public TNode? Insert(TNode? node, Word word);

}

public class AVLTreeService : IAVLTreeService {
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
}