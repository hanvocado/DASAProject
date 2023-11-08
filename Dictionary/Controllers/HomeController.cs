using System.Diagnostics;
using Microsoft.AspNetCore.Mvc;
using Dictionary.Models;
using Dictionary.Services;
using Dictionary.Models.ViewModels;
using Microsoft.AspNetCore.Http;

namespace Dictionary.Controllers;

public class HomeController : Controller
{
    public readonly IAVLTreeService _avlTree;
    public readonly IStackService _stack;
    public HomeController(IAVLTreeService aVLTree, IStackService stack)
    {
        _avlTree = aVLTree;
        _stack = stack;
    }

    public IActionResult Index(string? searchStr)
    {   
        ViewData["searchStr"] = searchStr;

        _stack.Searched(searchStr);
        
        Word? result = _avlTree.LookUp(searchStr);
        if (result != null) {
            _stack.Push(searchStr!);
        }

        return View(new IndexVM {
            Word = result,
            HistoryTop = _stack.GetHistory().Top
        });
    }
}
