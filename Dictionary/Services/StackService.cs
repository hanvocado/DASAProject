using Dictionary.Models;

namespace Dictionary.Services;

public interface IStackService {
    public void Push(string key);
    public Stack GetHistory();
    public void Searched(string? key);
    public string? Forwarded(string? currentKey);
    public string? Backwarded(string? currentKey);
}

public class StackService : IStackService
{
    private Stack History = new ();
    private Stack Forward = new Stack();
    private Stack Backward = new Stack();

    public bool IsEmpty(Stack stack)
    {
        return stack.Top == null;
    }
    public void Push(string key)
    {
        Node p = new(key);            
        p.Next = History.Top;
        History.Top = p;
    }
    private void push(Stack stack, string? key) {
        Node p = new(key);
        p.Next = stack.Top;
        stack.Top = p;
    }
    private void pop(Stack stack) {
        if (IsEmpty(stack))
            return;
        stack.Top = stack.Top!.Next;
    }
    private string? top(Stack stack) {
        if (IsEmpty(stack))
            return null;
        return stack.Top!.Key;
    }

    public Stack GetHistory() {
        return History;
    }

    public void Searched(string? key) {
        if (!String.Equals(top(Backward), key, StringComparison.OrdinalIgnoreCase)) {
            push(Backward, key);
        }
    }

    public string? Forwarded(string? currentKey) {
        if (!IsEmpty(Forward)) {
            push(Backward, currentKey);
        }
        string? keyToForwardTo = top(Forward);
        pop(Forward);
        Console.WriteLine("forward to " + keyToForwardTo);
        return keyToForwardTo;
    }
    public string? Backwarded(string? currentKey) {
        if (!IsEmpty(Backward)) {
            push(Forward, currentKey);
        }
        pop(Backward);
        string? keyToBackwardTo = top(Backward);
        pop(Backward);

        Console.WriteLine("backward to " + keyToBackwardTo);
        return keyToBackwardTo;
    }
}