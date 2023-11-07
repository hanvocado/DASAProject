using System.Diagnostics;
using Microsoft.AspNetCore.Mvc;
using Dictionary.Models;
using Dictionary.Services;

namespace Dictionary.Controllers;

public class HomeController : Controller
{
    public readonly IAVLTreeService _avlTree;
    public readonly IStackService _stack;
    public Stack SearchHistory = new ();
    public HomeController(IAVLTreeService aVLTree, IStackService stack)
    {
        _avlTree = aVLTree;
        _stack = stack;
    }

    public IActionResult Index(string? searchStr)
    {   
        ViewData["searchStr"] = searchStr;
        Word? result = _avlTree.LookUp(searchStr);
        if (result != null) {
            _stack.Push(SearchHistory, searchStr!);
        }

        return View(result);
    }

    public IActionResult History()
    {
        return View();   
    }
}
