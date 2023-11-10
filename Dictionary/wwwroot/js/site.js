$(document).ready(function () {
  $("#searchInput").on("input", function () {
    var query = $(this).val();

    // Send AJAX request to the server
    $.ajax({
      url: "/Home/Suggest", // Update with your actual controller and action
      type: "GET",
      data: { query: query },
      success: function (data) {
        // Update your UI with the suggestions received from the server
        displaySuggestions(data);
      },
      error: function (error) {
        console.error("Error fetching suggestions: ", error);
      },
    });
  });

  // Function to display suggestions in the suggestionList div
  function displaySuggestions(suggestions) {
    var suggestionList = $("#suggestions");
    suggestionList.empty(); // Clear previous suggestions

    // Display each suggestion in a list item
    for (var i = 0; i < Math.min(suggestions.length, 6); i++) {
      $.get(
        "/Home/SuggestionPartial",
        { model: suggestions[i] },
        function (partialHtml) {
          suggestionList.append(partialHtml);
        }
      );
    }
  }
});
