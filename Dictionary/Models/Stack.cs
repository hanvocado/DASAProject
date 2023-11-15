namespace Dictionary.Models;

public class Node {
    public string? Key { get; set; }
    public Node? Next { get; set; }

    public Node(string? key) {
        Key = key;
    }
}

public class Stack {
    private Node? top { get; set; }

    public Stack() {
        top = null;
    }

    public bool IsEmpty()
    {
        return top == null;
    }

    public void Pop()
    {
        if (IsEmpty())
            return;
        top = top!.Next;
    }
    public string? Top()
    {
        if (IsEmpty())
            return null;
        return top!.Key;
    }
    public void Push(string? key)
    {
        Node p = new(key);
        p.Next = top;
        top = p;
    }
}