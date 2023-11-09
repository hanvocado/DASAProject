namespace Dictionary.Models;

public class Node {
    public string? Key { get; set; }
    public Node? Next { get; set; }

    public Node(string? key) {
        Key = key;
    }
}

public class Stack {
    public Node? Top { get; set; }

    public Stack() {
        Top = null;
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
}