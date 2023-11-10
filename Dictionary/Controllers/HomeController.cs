using System.Diagnostics;
using Microsoft.AspNetCore.Mvc;
using Dictionary.Models;
using Dictionary.Services;
using Dictionary.Models.ViewModels;
using Microsoft.AspNetCore.Http;

namespace Dictionary.Controllers;

public class HomeController : Controller
{
    public readonly IDictionaryService _dictionary;
    public readonly IStackService _stack;
    public readonly IUserService _user;
    public HomeController(IDictionaryService dictionary, IStackService stack, IUserService user)
    {
        _dictionary = dictionary;
        _stack = stack;
        _user = user;
    }

    public IActionResult Index(string? searchStr)
    {   
        ViewData["searchStr"] = searchStr;

        _stack.Searched(searchStr);
        
        Word? result = _dictionary.LookUp(searchStr);

        return View(new IndexVM {
            Word = result,
            WordIsSaved = result == null ? false : _user.IsSaved(result.KeyWord),
            ForwardDisabled = _stack.ForwardDisabled(),
            BackwardDisabled = _stack.BackwardDisabled()
        });
    }
    public IActionResult SavedWords()
    {
        return View(_user.GetSavedWords());
    }

    public IActionResult Backward(string? currentKey)
    {
        return RedirectToAction(nameof(Index), new { searchStr = _stack.Backwarded(currentKey) });
    }
    public IActionResult Forward(string? currentKey)
    {
        return RedirectToAction(nameof(Index), new { searchStr = _stack.Forwarded(currentKey) });
    }
    public IActionResult SaveOrUnsave(string key)
    {
        _user.SaveOrUnsaveWord(key);
        return RedirectToAction(nameof(Index), new { searchStr = key });
    }
    public IActionResult Suggest(string query)
    {
        return Ok(_dictionary.Suggest(query));
    }
    public IActionResult SuggestionPartial(string model)
    {
        return PartialView("_Suggestion", model);
    }
}
