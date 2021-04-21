#include "ofApp.h"

void ofApp::setup()
{
	iota_app.setup(this);
}

void ofApp::audioOut(float* output, int bufferSize, int nChannels)
{
	iota_app.audio_out(output, bufferSize, nChannels);
}

void ofApp::update()
{
	iota_app.update();
}

void ofApp::draw()
{
	iota_app.draw();
}

void ofApp::keyPressed(int key)
{
	iota_app.key_pressed(key);
}

void ofApp::keyReleased(int key)
{
	iota_app.key_released(key);
}

void ofApp::mouseMoved(int x, int y)
{
	iota_app.mouse_moved(x, y);
}

void ofApp::mouseDragged(int x, int y, int button)
{
	iota_app.mouse_dragged(x, y, button);
}

void ofApp::mousePressed(int x, int y, int button)
{
	iota_app.mouse_pressed(x, y, button);
}

void ofApp::mouseScrolled(int x, int y, float scrollX, float scrollY)
{
	iota_app.mouse_scrolled(x, y, scrollX, scrollY);
}

void ofApp::mouseReleased(int x, int y, int button)
{
	iota_app.mouse_released(x, y, button);
}

void ofApp::mouseEntered(int x, int y)
{
	iota_app.mouse_entered(x, y);
}

void ofApp::mouseExited(int x, int y)
{
	iota_app.mouse_exited(x, y);
}

void ofApp::windowResized(int w, int h)
{
	iota_app.window_resized(w, h);
}

void ofApp::gotMessage(ofMessage msg)
{
	iota_app.got_message(msg);
}

void ofApp::dragEvent(ofDragInfo dragInfo)
{
	iota_app.drag_event(dragInfo);
}