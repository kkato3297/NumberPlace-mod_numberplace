$(function () {
	try {
		CreatePuzzle();
	} catch(e) {
		setTimeout(function () {
			document.location = document.location;
		}, 0);
	}
});

var g_cellList, g_cellListAns;

function CreatePuzzle() {
	LoadMap()
	.done(function (data) {
		g_cellList = data.List;
		g_cellListAns = data.Answer;
		MappingCells();
	})
	.fail(function (error) {
		throw error;
	});
}

function LoadMap() {
	var l_result = null;
	var l_defer = new $.Deferred;

	$.ajax({
		url: '/numberplace/create_puzzle.json',
		type: 'GET'
	})
	.done(function (data) {
		l_defer.resolve(data);
	})
	.fail(function (data) {
		l_defer.reject('create_puzzle.json: Connection Failed...');
	});

	return l_defer;
}

function MappingCells() {
	$('.cell').each(function (idx, el) {
		var val = g_cellList[idx];
		if (val) {
			$(el).text(val);
		} else {
			$(el).html('<input type="text" class="userInput" id="input' + idx + '" onchange="cellChanged(' + idx + ')" />');
		}
	});
}

function cellChanged(idx) {
	var isEmpty = $('#input' + idx).val().length === 0;
	var isCorrect = ('' + g_cellListAns[idx]) === $('#input' + idx).val();

	if (isEmpty || isCorrect) {
		$('#input' + idx).removeClass('Error');
	} else {
		$('#input' + idx).addClass('Error');
	}
}
