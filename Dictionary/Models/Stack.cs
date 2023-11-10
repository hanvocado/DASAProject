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

public class LinkedList
{
    public Node? Head { get; set; }
    public Node? Tail { get; set; }

    public LinkedList()
    {
        Head = null;
        Tail = null;
    }

    public void AddLast(string key)
    {
        Node node = new Node(key);
        if (Head == null)
        {
            Head = node;
            Tail = node;
        }
        else
        {
            Tail!.Next = node;
            Tail = node;
        }
    }
    public Node? Find(string key)
    {
        for (Node? p = Head; p != null; p = p.Next)
        {
            if (String.Equals(key, p.Key, StringComparison.OrdinalIgnoreCase))
            {
                return p;
            }
        }
        return null;
    }

    public void Remove(string key) {
        Node? nodeToRemove = Find(key);
        if (nodeToRemove == null) return;
        if (Head == null) return;
        if (Head == nodeToRemove)
        {
            if (Tail == nodeToRemove)
            {
                Head = Tail = null;
            }
            else
            {
                Head = Head.Next;
            }
        }
        else
        {
            Node? q = Head;
            while (q != null && q.Next != nodeToRemove)
            {
                q = q.Next;
            }
            if (q == null) return;

            q.Next = nodeToRemove.Next;
            if (Tail == nodeToRemove)
                Tail = q;
        }
        nodeToRemove = null;
    }
}