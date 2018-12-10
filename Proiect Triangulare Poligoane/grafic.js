var POLYGON_POINTS = 0;
var SEARCHED_POINT = 1;
var TRIANGLES = 2;

var SCALESIZE = 100;

var trAnim;

/* 
	Clasa graphics tine tot ce trebuie pentru a desena
	segment, punct, poligon(fill sau doar contur)
*/
class Graphics {
	constructor() {
		this.drawingOffsetX = 0; 
		this.drawingOffsetY = 0; 
		this.canvas = document.getElementById("graphic");
		
		this.initCanvas(this.canvas);
		
		this.zoom = 0;
		this.zoomed = false;
		
		this.lastCoords;
		
		this.history = [];
		
		this.DRAWSEG = 0;
		this.DRAWPIX = 1;
		this.STROKEPOLY = 2;
		this.FILLPOLY = 3;
		
		this.PIXELSIZE = 10;
		
		this.boundDragMove = this.setDragMove.bind(this);
		
		window.addEventListener("wheel", this.setZoom.bind(this));
		this.canvas.addEventListener("mousedown", this.setDragStart.bind(this));
		this.canvas.addEventListener("mouseup", this.setDragEnd.bind(this));
	}
	
	setDragStart(e) {
		e.preventDefault();
		this.lastCoords = {x : e.clientX, y : e.clientY};
		this.canvas.addEventListener("mousemove", this.boundDragMove);
	}
	
	setDragMove(e) {
		var deltaX = this.lastCoords.x - e.clientX;
		var deltaY = this.lastCoords.y - e.clientY;
		this.lastCoords = {x : e.clientX, y : e.clientY};
		
		this.ctx.restore();
		
		this.ctx.clearRect(0, 0, this.canvas.width, this.canvas.height);
		
		this.ctx.save();
		
		this.setOffset(-deltaX, -deltaY);
		
		if (this.zoom < 0)
			this.ctx.scale(1 / (-this.zoom), 1 / (-this.zoom));
		else
			this.ctx.scale(1 + this.zoom, 1 + this.zoom);
		
		this.redrawCanvas();
	}
	
	setDragEnd() {
		this.canvas.removeEventListener("mousemove", this.boundDragMove)
	}
	
	//offsetuieste tot graficul(pentru centrare)
	setOffset(xValue, yValue) {
		if (this.drawingOffsetX === 0 && this.drawingOffsetY === 0 && this.zoom == 0)
			this.ctx.save();
		
		this.drawingOffsetX += xValue; 
		this.drawingOffsetY += yValue; 
		
		this.ctx.translate(this.drawingOffsetX, this.drawingOffsetY);
	}
	
	setZoom(e) {
		if (this.zoomed)
			return;
		
		this.ctx.restore();
		
		this.ctx.clearRect(0, 0, this.canvas.width, this.canvas.height);
		
		this.ctx.save();
		
		//reoffsetuiesc canvas-ul
		this.ctx.translate(this.drawingOffsetX, this.drawingOffsetY);
		
		this.zoomed = true;
		
		var clearZoom = function() {this.zoomed = false;};
		
		setTimeout(clearZoom.bind(this), 200);
		
		//daca deltaY e negativ avem wheelUP deci vrem zoom in
		this.zoom += (e.deltaY < 0)? 1 : -1;
		
		if (this.zoom < 0)
			this.ctx.scale(1 / (-this.zoom), 1 / (-this.zoom));
		else
			this.ctx.scale(1 + this.zoom, 1 + this.zoom);
		
		this.redrawCanvas();
	}
	
	redrawCanvas() {
		for (var i = 0; i < this.history.length; i++) {
			var H = this.history[i];
			if (H.action === this.DRAWSEG)
				this.drawSegment(H.pt1, H.pt2, H.color, true);
			
			if (H.action === this.DRAWPIX)
				this.drawPixel(H.pt, H.color, true);
			
			if (H.action === this.STROKEPOLY)
				this.strokePolygon(H.poly, H.color, true);
			
			if (H.action === this.FILLPOLY)
				this.fillPolygon(H.poly, H.color, true);
		}
	}
	
	initCanvas(canvas) {
		canvas.width = document.body.clientWidth * 0.8;
		canvas.height = document.body.clientHeight * 0.5;
		
		if (!this.ctx) {
			this.ctx = canvas.getContext("2d");
			//this.ctx.scale(1, -1);
		}
	}
	
	setPixelSize(size) {
		this.PIXELSIZE = size;
	}
	
	//primeste doua puncte si deseneaza segmentul dintre ele
	drawSegment(pt1, pt2, color, redrawing = false) {
		if (!redrawing)
			this.history.push({action : this.DRAWSEG, pt1 : pt1, pt2 : pt2, color : color});
		
		this.ctx.beginPath();
		this.ctx.moveTo(pt1.x, pt1.y);
		this.ctx.lineTo(pt2.x, pt2.y);
		this.ctx.closePath();
		
		this.ctx.strokeStyle = color;
		this.ctx.stroke();
	}
	
	//primeste coordonatele punctului si culoarea pentru a-l desena
	drawPixel(pt, color, redrawing = false) {
		if (!redrawing)
			this.history.push({action : this.DRAWPIX, pt : pt, color : color});
		
		this.ctx.fillStyle = color;
		this.ctx.beginPath();
		
		this.ctx.arc(pt.x, pt.y, this.PIXELSIZE / 2, 0, Math.PI * 2);
		this.ctx.closePath();
		this.ctx.fill();
	}
	
	/*
		traseaza conturul unui poligon
		primeste un obiect din clasa Polygon si parcurge array-ul
		de puncte al acestuia trasand segmentele dintre ele
	*/
	strokePolygon(poly, color, redrawing = false) {
		if (!redrawing)
			this.history.push({action : this.STROKEPOLY, poly : poly, color : color});
		
		for (var i = 0; i < poly.arr.length - 1; i++)
			this.drawSegment(poly.arr[i], poly.arr[i + 1], color, true);
		this.drawSegment(poly.arr[poly.arr.length - 1], poly.arr[0], color, true);
	}
	
	/*
		primeste Polygon object si il umple
		cu culoarea primita ca parametru
	*/
	fillPolygon(poly, color, redrawing = false) {
		if (!redrawing)
			this.history.push({action : this.FILLPOLY, poly : poly, color : color});
		
		//daca poligonul este doar un segment 
		//atunci trasez un segment
		if (poly.arr.length == 2)
			return this.drawSegment(poly.arr[0], poly.arr[1], color, true);
		
		this.ctx.beginPath();
		this.ctx.moveTo(poly.arr[0].x, poly.arr[0].y);
		for (var i = 1; i < poly.arr.length; i++)
			this.ctx.lineTo(poly.arr[i].x, poly.arr[i].y);
		this.ctx.lineTo(poly.arr[0].x,poly.arr[0].y);
		this.ctx.closePath();
		
		this.ctx.fillStyle = color;
		this.ctx.fill();
	}
}

/*
	Clasa poligon tine punctele ordonate ale unui poligon
*/
class Polygon {
	constructor(ptsArray) {
		this.arr = ptsArray;
		this.scaleCoords();
	}
	
	scaleCoords() {
		//maresc coordonatele uniform pentru a avea 
		//un desen la rezolutie mai mare
		for (var i = 0; i < this.arr.length; i++) {
			this.arr[i].x *= SCALESIZE;
			this.arr[i].y *= SCALESIZE;
		}
	}
}

/* 
	Clasa tringulationAnimator parseaza un fisier JSON
	tine : butonul de step, elementul de log(unde afiseaza mesaje) 
	si graficul pe care il apeleaza pentru a desena diverse forme geometrice
	in functie de cum le gaseste in fisierul JSON
*/
class TriangulationAnimator {
	constructor(jsonData) {
		this.graph = new Graphics();
		
		this.stepButton = document.getElementById("step");
		
		this.boundStepper = this.nextStep.bind(this);
		this.stepButton.addEventListener("click", this.boundStepper);
		this.logger = document.getElementById("logger");
		
		//arrayul de date din JSON-ul parsat
		this.stepsArray = JSON.parse(jsonData);
		
		//tin un counter de step-uri
		this.currStep = 0;
		
		//la fiecare pas afisez triunghi si dupa la pasul urmator
		//daca punctul se alfa in triunghi sau nu
		this.showNewTriangle = true;
		this.lastTriangle;
		
		//poligon initial
		this.polygon = new Polygon(this.stepsArray[POLYGON_POINTS]);
		//punctul cautat
		this.searchedPoint = this.stepsArray[SEARCHED_POINT];
		//array-ul de array-uri de puncte
		this.trianglesArr = this.stepsArray[TRIANGLES];
		
		this.initPolygon();
	}
	
	//partea de initializare a graficului
	initPolygon() {
		/*
			determin y minim si x minim ca sa offsetuiesc graficul
			a.i. sa intre in intregime in ecran
		*/
		var polyPts = this.polygon.arr;
		var minX = polyPts[0].x, minY = polyPts[0].y;
		
		for (var i = 1; i < polyPts.length; i++) {
			if (polyPts[i].x < minX)
				minX = polyPts[i].x;
			
			if (polyPts[i].y < minY)
				minY = polyPts[i].y;
		}
		
		this.graph.setOffset(-minX + 10, -minY + 10);
		
		this.graph.strokePolygon(this.polygon, "black");
		this.logMessage("Poligonul simplu de triangulat");
		
		this.logMessage("Punctul de coordonate (" + this.searchedPoint.x + ", " + this.searchedPoint.y + ")");
		this.searchedPoint.x *= SCALESIZE;
		this.searchedPoint.y *= SCALESIZE;
		this.graph.drawPixel(this.searchedPoint, "red");
	}
	
	logMessage(msg) {
		this.logger.innerHTML += msg + "<br/>";
	}
	
	//de fiecare data cand apasam pe buton continuam algoritmul
	//desenand triunghiuri/segmente/afisand mesaje corespunzatoare
	nextStep() {
		if (this.currStep < this.trianglesArr.length) {
			//daca sunt in faza de desenare a unui triunghi nou
			if (this.showNewTriangle) {
				var trianglePts = this.trianglesArr[this.currStep].slice(0, this.trianglesArr[this.currStep].length - 1);
				this.lastTriangle = new Polygon(trianglePts);
			
				this.graph.strokePolygon(this.lastTriangle, "black");
				this.graph.drawPixel(this.searchedPoint, "red");
			}
			else {
				//daca ultima pozitie din array-ul de puncte ce defineste triunghiul este "true"
				if (this.trianglesArr[this.currStep][this.trianglesArr[this.currStep].length - 1]) {
					this.graph.fillPolygon(this.lastTriangle, "green");
					this.logMessage("Acest triunghi contine punctul cautat!");
					this.graph.drawPixel(this.searchedPoint, "red");
				}
				else {
					this.graph.strokePolygon(this.lastTriangle, "red");
					this.logMessage("Punctul cautat nu este in acest triunghi.");
					this.graph.drawPixel(this.searchedPoint, "red");	
				}
				this.currStep++;
			}
			//daca am afisat un triunghi nou la pasul urmator
			//afisez daca punctul se afla in triunghi sau nu
			this.showNewTriangle = !this.showNewTriangle;
		}
		else {
			this.logMessage("Sfarsitul algoritmului");
			this.stepButton.removeEventListener("click", this.boundStepper);
		}
	}
}

function getJSON(callback) {
	var xobj = new XMLHttpRequest();
	xobj.overrideMimeType("application/json");
	xobj.open("GET", "data.json", true);
	
	xobj.onreadystatechange = function() {
		if (xobj.readyState === 4 && xobj.status === 200)
			callback(xobj.responseText);
	}
	
	xobj.send(null);
}

function init() {
	//trimit cerere dupa fisierul data.json, la raspuns init TriangulationAnimator
	getJSON(function(response) {window.trAnim = new TriangulationAnimator(response);});
}