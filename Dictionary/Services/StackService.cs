using Dictionary.Models;

namespace Dictionary.Services;

public interface IStackService {
    public void Searched(string? key);
    public string? Forwarded(string? currentKey);
    public string? Backwarded(string? currentKey);
    public bool ForwardDisabled();
    public bool BackwardDisabled();
}

public class StackService : IStackService
{
    private Stack Forward = new Stack();
    private Stack Backward = new Stack();

    public bool IsEmpty(Stack stack)
    {
        return stack.Top == null;
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

        return keyToForwardTo;
    }
    public string? Backwarded(string? currentKey) {
        if (!IsEmpty(Backward)) {
            push(Forward, currentKey);
        }
        pop(Backward);
        string? keyToBackwardTo = top(Backward);
        pop(Backward);

        return keyToBackwardTo;
    }

    public bool ForwardDisabled()
    {
        return IsEmpty(Forward);
    }

    public bool BackwardDisabled()
    {
        return IsEmpty(Backward);
    }
}