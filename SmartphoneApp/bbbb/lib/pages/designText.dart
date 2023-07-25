import 'package:flutter/material.dart';

class DesignText extends StatefulWidget {
  final Color color;
  final double bLevel;
  final Offset offsetW;
  final double height1;
  final Offset offsetB;
  final double width1;
  final IconData iconData;
  final double iconSize;
  final Color pressed;
  final String text;
  final double radius;
  final double radius1;
  final double radius2;
  final double radius3;
  final double radius4;
  final FontWeight weight;

  static bool isPressed=false;

  const DesignText(
      {Key key,
        this.color,
        this.offsetW,
        this.bLevel,
        this.height1,
        this.width1,
        this.offsetB,
        this.iconData,
        this.iconSize,
        this.pressed,
        this.text,
        this.radius,
        this.radius1,
        this.radius2,
        this.radius3,
        this.radius4,
        this.weight,
      })
      : super(key: key);
  @override
  _DesignTextState createState() => _DesignTextState();
}

class _DesignTextState extends State<DesignText> {
  //bool isPressed = false;

  void onPressedUp(PointerUpEvent event) {
    setState(() {
      DesignText.isPressed = false;
    });
  }

  void onPressedDown(PointerDownEvent event) {
    setState(() {
      DesignText.isPressed = true;
    });
  }

  @override
  Widget build(BuildContext context) {
    return Listener(
      onPointerUp: onPressedUp,
      onPointerDown: onPressedDown,
      child: DesignText.isPressed
          ? Container(
        height: widget.height1,
        width: widget.width1,
        decoration: BoxDecoration(
            color: widget.color,
            borderRadius: BorderRadius.only(topLeft: Radius.circular(widget.radius1),topRight: Radius.circular(widget.radius2),bottomLeft: Radius.circular(widget.radius3),bottomRight: Radius.circular(widget.radius4)),
            boxShadow: [
              BoxShadow(
                  blurRadius: 5.0,
                  offset: Offset(-3, -3),
                  color: Colors.white.withOpacity(.7)),
              BoxShadow(
                  blurRadius: 5.0,
                  offset: Offset(3, 3),
                  color: Colors.black.withOpacity(.15))
            ]),
        child: Padding(
          padding: const EdgeInsets.all(3.0),
          child: Container(
            decoration: BoxDecoration(
              color: widget.color,
              borderRadius: BorderRadius.only(topLeft: Radius.circular(widget.radius1),topRight: Radius.circular(widget.radius2),bottomLeft: Radius.circular(widget.radius3),bottomRight: Radius.circular(widget.radius4)),
            ),
            child: Padding(
              padding: const EdgeInsets.all(2.0),
              child: Container(
                decoration: BoxDecoration(
                    color: widget.color,
                    borderRadius: BorderRadius.only(topLeft: Radius.circular(widget.radius1),topRight: Radius.circular(widget.radius2),bottomLeft: Radius.circular(widget.radius3),bottomRight: Radius.circular(widget.radius4)),
                    boxShadow: [
                      BoxShadow(
                          blurRadius: widget.bLevel,
                          offset: widget.offsetW,
                          color: Colors.white.withOpacity(.7)),
                      BoxShadow(
                          blurRadius: widget.bLevel,
                          offset: widget.offsetB,
                          color: Colors.black.withOpacity(.25))
                    ]),
                child: Center(
                    child:  Text(
                        widget.text,
                        textAlign: TextAlign.center,
                        style: TextStyle(
                            fontWeight: widget.weight,
                            fontSize: widget.iconSize,
                            color: Colors.black.withOpacity(0.6)))
                ),
              ),
            ),
          ),
        ),
      )
          : Container(
        height: widget.height1,
        width: widget.width1,
        decoration: BoxDecoration(
            color: Color(0xFFe6ebf2),
            borderRadius: BorderRadius.only(topLeft: Radius.circular(widget.radius1),topRight: Radius.circular(widget.radius2),bottomLeft: Radius.circular(widget.radius3),bottomRight: Radius.circular(widget.radius4)),
            boxShadow: [
              BoxShadow(
                  blurRadius: 5.0,
                  offset: Offset(-3, -3),
                  color: Colors.white.withOpacity(.7)),
              BoxShadow(
                  blurRadius: 5.0,
                  offset: Offset(3, 3),
                  color: Colors.black.withOpacity(.15))
            ]),
        child: Center(
            child: Text(
                widget.text,
                textAlign: TextAlign.center,
                style: TextStyle(
                  fontWeight: widget.weight,
                  fontSize: widget.iconSize,
                  color: widget.pressed,))
        ),

      ),
    );
  }
}