namespace Dictionary.Models;

public class LNode {
    public Word Word { get; set; }
    public LNode? Next { get; set; }

    public LNode(Word word) {
        Word = word;
    }
}
public class LinkedList
{
    public LNode? Head { get; set; }
    public LNode? Tail { get; set; }

    public LinkedList()
    {
        Head = null;
        Tail = null;
    }

    public void AddLast(Word word)
    {
        LNode node = new LNode(word);
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
    public LNode? Find(string key)
    {
        for (LNode? p = Head; p != null; p = p.Next)
        {
            if (String.Equals(key, p.Word.KeyWord, StringComparison.OrdinalIgnoreCase))
            {
                return p;
            }
        }
        return null;
    }

    public void Remove(string key)
    {
        LNode? nodeToRemove = Find(key);
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
            LNode? q = Head;
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