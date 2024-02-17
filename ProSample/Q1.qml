import QtQuick 2.7

Text {
    id: label
    x: 24; y: 24
    // 自定义属性，表示空格按下的次数
    property int spacePresses: 0
    text: "Space pressed: " + spacePresses + " times"
    // (1) 文本变化的响应函数
    onTextChanged: console.log("text changed to:", text)
    // 接收键盘事件，需要设置 focus 属性
    focus: true
    // (2) 调用 JavaScript 函数
    Keys.onSpacePressed: {
        increment()
    }
    // 按下 Esc 键清空文本
    Keys.onEscapePressed: {
        label.text = ''
    }
    // (3) 一个 JavaScript 函数
    function increment() {
        spacePresses = spacePresses + 1
    }
}
