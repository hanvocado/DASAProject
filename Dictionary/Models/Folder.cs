namespace Dictionary.Models;

public class Folder {
    public string Name { get; set; }
    public List<string> ChildrenFolder { get; set; } = new();
    public LinkedList Items { get; set; } = new();
    public Folder(string name) {
        Name = name;
    }
    public Folder(string name, LinkedList items) {
        Name = name;
        Items = items;
    }

    public void AddChildFolder(string childName) {
        ChildrenFolder.Add(childName);
    }
    public void DeleteChildFolder(string childName) {
        ChildrenFolder.Remove(childName);
    }
    // public Folder GetChildFolder(string childName) {

    // }
    public void AddItem(string key) {
        Items.AddLast(key);
    }
    public void RemoveItem(string key) {
        Node? nodeToRemove = Items.Find(key);
        Items.Remove(nodeToRemove);
    }

}