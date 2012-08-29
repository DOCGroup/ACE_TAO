var IMG_EXPANDED  = 'Expanded.png';
var IMG_COLLAPSED = 'Collapsed.png';
var IMG_BLANK = 'blank.png';
var IMG_LEAF = 'LeafRowHandle.png';

new Image(9,9),src = IMG_EXPANDED; // caching
new Image(9,9),src = IMG_COLLAPSED; // caching
new Image(9,9),src = IMG_BLANK; // caching
new Image(9,9),src = IMG_LEAF; // caching

function hover(iNode, over) {

    if (over) {
        t = document.getElementById(iNode).alt;
        
        if (t == '*') {
            document.getElementById(iNode).src=IMG_LEAF;
        } else if (t == 'V') {
            document.getElementById(iNode).src=IMG_EXPANDED;
        } else {
            document.getElementById(iNode).src=IMG_COLLAPSED;
        }
    
    } else {
        document.getElementById(iNode).src=IMG_BLANK;
    }
}

function expand(ioNode) {
    ioWedge = "i" + ioNode.substr(1);

    if (document.getElementById && document.getElementById(ioNode) !=  null) {

        document.getElementById(ioNode).className='expanded';

        if (document.getElementById(ioWedge) !=  null) {
            document.getElementById(ioWedge).src=IMG_EXPANDED;
            document.getElementById(ioWedge).title='collapse';
            document.getElementById(ioWedge).alt='V';
        }
    }
}

function collapse(ioNode) {
    ioWedge = "i" + ioNode.substr(1);

    if (document.getElementById && document.getElementById(ioNode) != null) {

        document.getElementById(ioNode).className='collapsed';

        if (document.getElementById(ioWedge) !=  null) {
            document.getElementById(ioWedge).src=IMG_COLLAPSED;
            document.getElementById(ioWedge).title='expand';
            document.getElementById(ioWedge).alt='>';
        }
    }
}

function ioSwitch(ioNode,fully) {

    if (document.getElementById && document.getElementById(ioNode) !=  null) {
        nodeState = document.getElementById(ioNode).className;
    }

    if (nodeState == 'collapsed') {
        if (fully) {
            expandAll();
        } else {
            expand(ioNode);
        }
    }

    else {
        if (fully) {
            collapseAll();
        } else {
            collapse(ioNode);
        }
    }
    updateRowBackgroundColors();
}

function expandAll() {

    if (document.getElementsByTagName) {
        nodeList = document.getElementsByTagName('div');

        for (var i = 0; i < nodeList.length; i++) {
    
            if (nodeList.item(i).className == 'expanded' || nodeList.item(i).className == 'collapsed') {
                expand(nodeList.item(i).id);
            }
        }
    }

    else {
        alert ("Sorry, don't know how to make this run in your browser.");
    }
}

function collapseAll() {

    if (document.getElementsByTagName) {
        nodeList = document.getElementsByTagName('div');

        for (var i = 0; i < nodeList.length; i++) {
    
            if (nodeList.item(i).className == 'expanded' || nodeList.item(i).className == 'collapsed') {
                collapse(nodeList.item(i).id);  
            }
        }
    }

    else {
        alert ("Sorry, don't know how to make this run in your browser.");
    }
}

//
// alternate background color support
//
function hasClass(ele,cls) {
    return ele.className.match(new RegExp('(\\s|^)'+cls+'(\\s|$)'));
}
function addClass(ele,cls) {
    if (!this.hasClass(ele,cls)) ele.className += " "+cls;
}
function removeClass(ele,cls) {
    if (hasClass(ele,cls)) {
        var reg = new RegExp('(\\s|^)'+cls+'(\\s|$)');
        ele.className=ele.className.replace(reg,' ');
    }
}

function isVisible(obj)
{
    if (obj == document)
        return true;

    if (!obj)
        return false;
    if (!obj.parentNode)
        return false;
    if (obj.style) {
        if (obj.style.display == 'none')
            return false;
        if (obj.style.visibility == 'hidden')
            return false;
    }

    //Try the computed style in a standard way
    if (window.getComputedStyle) {
        var style = window.getComputedStyle(obj, "");
        if (style.display == 'none')
            return false;
        if (style.visibility == 'hidden')
            return false;
    }

    //Or get the computed style using IE's silly proprietary way
    var style = obj.currentStyle
    if (style) {
        if (style['display'] == 'none')
            return false;
        if (style['visibility'] == 'hidden')
            return false;
    }
    return isVisible(obj.parentNode);
}

function updateRowBackgroundColors() {
    // do not update row colors unless alternate row colors has been specified.
    if (!hasClass(document.body, 'altRowColors'))
        return;
    var possibleRows = document.getElementsByClassName("row");
    var rowCount = 0;
    for(var i = 0; i < possibleRows.length; i++) {
        var element = possibleRows[i];
        if (isVisible(element)) {
            rowCount++;
            if (rowCount % 2 == 0) {
                addClass(element, 'altRow');
            } else {
                removeClass(element, 'altRow');
            }
        }
    }
}

window.onload = updateRowBackgroundColors;
