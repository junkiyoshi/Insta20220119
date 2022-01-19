#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(30);
	ofSetWindowTitle("openFrameworks");

	ofBackground(255);
	ofEnableDepthTest();
	//ofEnableBlendMode(ofBlendMode::OF_BLENDMODE_MULTIPLY);

	this->base_line.setMode(ofPrimitiveMode::OF_PRIMITIVE_LINES);
	this->draw_line.setMode(ofPrimitiveMode::OF_PRIMITIVE_LINES);

	float phi_deg_step = 2;
	float theta_deg_step = 8;

	float R = 250;
	float r = R * 0.25;

	for (float phi_deg = 0; phi_deg < 360; phi_deg += phi_deg_step) {

		for (float theta_deg = 0; theta_deg < 360; theta_deg += theta_deg_step) {

			auto index = this->base_face.getNumVertices();
			vector<glm::vec3> vertices;

			vertices.push_back(glm::vec3(this->make_point(R, r, theta_deg - theta_deg_step * 0.5, phi_deg - phi_deg_step * 0.5)));
			vertices.push_back(glm::vec3(this->make_point(R, r, theta_deg + theta_deg_step * 0.5, phi_deg - phi_deg_step * 0.5)));
			vertices.push_back(glm::vec3(this->make_point(R, r, theta_deg - theta_deg_step * 0.5, phi_deg + phi_deg_step * 0.5)));
			vertices.push_back(glm::vec3(this->make_point(R, r, theta_deg + theta_deg_step * 0.5, phi_deg + phi_deg_step * 0.5)));

			this->base_face.addVertices(vertices);
			this->base_line.addVertices(vertices);

			this->base_face.addIndex(index + 0); this->base_face.addIndex(index + 1); this->base_face.addIndex(index + 3);
			this->base_face.addIndex(index + 0); this->base_face.addIndex(index + 3); this->base_face.addIndex(index + 2);

			this->base_line.addIndex(index + 0); this->base_line.addIndex(index + 1);
			this->base_line.addIndex(index + 1); this->base_line.addIndex(index + 3);
			this->base_line.addIndex(index + 0); this->base_line.addIndex(index + 2);
			this->base_line.addIndex(index + 2); this->base_line.addIndex(index + 3);
		}
	}
}

//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);

	this->draw_face = this->base_face;
	this->draw_line = this->base_line;

	auto noise_seed = glm::vec3(ofRandom(1000), ofRandom(1000), ofRandom(1000));
	for (int i = 0; i < this->draw_face.getNumVertices(); i++) {

		auto vertex = this->draw_face.getVertex(i);

		auto rotation_x = glm::rotate(glm::mat4(), ofMap(ofNoise(glm::vec4(vertex * 0.003, noise_seed.x + ofGetFrameNum() * 0.02)), 0, 1, -30, 30) * (float)DEG_TO_RAD, glm::vec3(1, 0, 0));
		auto rotation_y = glm::rotate(glm::mat4(), ofMap(ofNoise(glm::vec4(vertex * 0.003, noise_seed.y + ofGetFrameNum() * 0.02)), 0, 1, -30, 30) * (float)DEG_TO_RAD, glm::vec3(0, 1, 0));
		auto rotation_z = glm::rotate(glm::mat4(), ofMap(ofNoise(glm::vec4(vertex * 0.003, noise_seed.z + ofGetFrameNum() * 0.02)), 0, 1, -30, 30) * (float)DEG_TO_RAD, glm::vec3(0, 0, 1));
			
		vertex = glm::vec4(vertex, 0) * rotation_z * rotation_y * rotation_x;

		this->draw_face.setVertex(i, vertex);
		this->draw_line.setVertex(i, vertex);
	}

}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();

	ofSetColor(255);
	this->draw_line.draw();

	ofSetColor(0);
	this->draw_face.draw();

	this->cam.end();
}

//--------------------------------------------------------------
glm::vec3 ofApp::make_point(float R, float r, float u, float v) {

	// 数学デッサン教室 描いて楽しむ数学たち　P.31

	u *= DEG_TO_RAD;
	v *= DEG_TO_RAD;

	auto x = (R + r * cos(u)) * cos(v);
	auto y = (R + r * cos(u)) * sin(v);
	auto z = r * sin(u);

	return glm::vec3(x, y, z);
}


//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}