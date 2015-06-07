
#include "cinder/app/AppNative.h"
#include "cinder/ImageIo.h"
#include "cinder/Camera.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/gl/Light.h"


using namespace ci;
using namespace ci::app;

// 
// わざわざ分ける理由が無いので、宣言と実装を一緒に書いてある:D
// 
class CinderTemplateApp : public AppNative {
  gl::Texture image;

  CameraPersp camera;

  std::unique_ptr<gl::Light> light;

  float rx, ry;

  

public:
  void prepareSettings(Settings* settings) override {
#if defined(CINDER_COCOA_TOUCH)
    // iOS:縦横画面両対応
    // boost::signals2を使った実装なので、他の挙動も
    // 同じように実装すれば良い
    getSignalSupportedOrientations().connect([](){ return InterfaceOrientation::All; });
#endif
  }
  
	void setup() override {
    image = loadImage(loadAsset("miku.png"));

    camera = CameraPersp(getWindowWidth(), getWindowHeight(),
                         35.0f,
                         1.0f, 500.0f);

    camera.lookAt(Vec3f(0.0f, 0.0f, 50.0f),
                  Vec3f(0.0f, 0.0f, 0.0f));

    // gl::Lightにはデフォルトコンストラクタが無いので
    // スマポを使って初期化
    light = std::unique_ptr<gl::Light>(new gl::Light(gl::Light::Type::DIRECTIONAL, 0));
    light->setDirection(Vec3f(0.0f, 0.0f, 1.0f));
    light->setDiffuse(Color(0.8f, 0.8f, 0.8f));
    light->setAmbient(Color(0.2f, 0.2f, 0.2f));

    gl::enable(GL_LIGHTING);
    gl::enable(GL_NORMALIZE);
    light->enable();

    glEnable(GL_COLOR_MATERIAL);

    gl::enable(GL_CULL_FACE);

    gl::enableDepthWrite();
    gl::enableDepthRead();

    rx = ry = 0.0f;
  }

  
  void resize() override {
    // リサイズ時は縦横比を更新すればオッケー
    float aspect_ratio = float(getWindowWidth()) / float(getWindowHeight());
    camera.setAspectRatio(aspect_ratio);
  }

  
	void draw() override {
    rx += 1.0f;
    ry += 1.0f;

    
    gl::clear(Color(0.5f, 0.5f, 0.5f));

    gl::setMatrices(camera);

    // 色つき立方体を描画
    gl::pushModelView();
    gl::translate(Vec3f(10.0f, 0.0f, 0.0f));
    gl::rotate(Vec3f(rx, ry, 0.0f));
    gl::scale(10.0f, 10.0f, 10.0f);

    gl::disable(GL_TEXTURE_2D);
    gl::color(Color(0.0f, 0.0f, 1.0f));
    gl::drawCube(Vec3f(0.0f, 0.0f, 0.0f), Vec3f(1.0f, 1.0f, 1.0f));
    gl::popModelView();

    // テクスチャつき立方体を描画
    gl::pushModelView();
    gl::translate(Vec3f(-10.0f, 0.0f, 0.0f));
    gl::rotate(Vec3f(rx, ry, 0.0f));
    gl::scale(10.0f, 10.0f, 10.0f);

    gl::enable(GL_TEXTURE_2D);
    image.bind();
    gl::color(Color(1.0f, 1.0f, 1.0f));
    gl::drawCube(Vec3f(0.0f, 0.0f, 0.0f), Vec3f(1.0f, 1.0f, 1.0f));
    image.unbind();
    gl::popModelView();
  }
};

CINDER_APP_NATIVE( CinderTemplateApp, RendererGl )
