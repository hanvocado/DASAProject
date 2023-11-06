using System.Diagnostics;
using Microsoft.AspNetCore.Mvc;
using Dictionary.Models;

namespace Dictionary.Controllers;

public class HomeController : Controller
{
    public HomeController()
    {
    }

    public IActionResult Index()
    {
        return View();
    }
}
