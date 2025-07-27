package dev.trindadedev.mygame;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Path;
import android.graphics.Rect;
import android.graphics.RectF;
import android.util.AttributeSet;
import android.view.GestureDetector;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.View;
import android.os.Handler;

// Basic GamePadView for MyGame Game.
// @see {interface#GamePadListener} at file end.
// Author: Aquiles Trindade (trindadedev).
public final class MyGamePadView extends View {
  private GamePadConfig mGamePadConfig;
  private GamePadListener mGamePadListener;

  private GestureDetector mGestureDetector;

  private Paint mPaint;
  private Paint mRipplePaint;

  private RectF mUpRect;
  private RectF mLeftRect;
  private RectF mDownRect;
  private RectF mRightRect;
  private RectF mRippleRect;
  private RectF mRippleClipRect = null;

  private int mRippleAlpha = 100;
  private boolean mIsTouchDown;
  private Vec2f mTouchPos;

  private final Handler mHandler = new android.os.Handler();
  private final Runnable mHoldRunnable = new Runnable() {
    @Override
    public void run() {
      if (mIsTouchDown && mGamePadListener != null) {
        int key = getKeyCodeEvent(mTouchPos);
        if (key != -1) mGamePadListener.onHold(key);
        mHandler.postDelayed(this, 100);
      }
    }
  };

  public MyGamePadView(final Context context) {
    super(context);
    init(context, null, -1, -1);
  }

  public MyGamePadView(final Context context, AttributeSet attrSet) {
    super(context, attrSet);
    init(context, attrSet, -1, -1);
  }

  public MyGamePadView(final Context context, AttributeSet attrSet, int styleResId) {
    super(context, attrSet, styleResId);
    init(context, attrSet, styleResId, -1);
  }

  public MyGamePadView(
          final Context context, AttributeSet attrSet, int styleResId, int attrResId) {
    super(context, attrSet, styleResId, attrResId);
    init(context, attrSet, styleResId, attrResId);
  }

  private final void init(
          final Context context, AttributeSet attrSet, int styleResId, int attrResId) {
    mPaint = new Paint();
    mPaint.setColor(Color.GRAY);
    mPaint.setStyle(Paint.Style.FILL);
    mPaint.setAntiAlias(true);

    mGamePadConfig = new GamePadConfig();

    mRipplePaint = new Paint();
    mRipplePaint.setColor(mGamePadConfig.buttonRippleColor);
    mRipplePaint.setAlpha(mRippleAlpha);
    mRipplePaint.setAntiAlias(true);

    mUpRect = new RectF();
    mLeftRect = new RectF();
    mDownRect = new RectF();
    mRightRect = new RectF();
    mRippleRect = new RectF();
    mTouchPos = new Vec2f(0f, 0f);

    mGestureDetector = new GestureDetector(context, new GestureDetector.SimpleOnGestureListener() {
      @Override
      public boolean onDoubleTap(final MotionEvent event) {
        final Vec2f pos = new Vec2f(event.getX(), event.getY());
        final int key = getKeyCodeEvent(pos);
        if (mGamePadListener != null && key != -1) mGamePadListener.onDoublePress(key);
        return true;
      }
    });
  }

  public final void setGamePadListener(final GamePadListener gamePadListener) {
    mGamePadListener = gamePadListener;
  }

  public final void setGamePadConfig(final GamePadConfig gamePadConfig) {
    mGamePadConfig = gamePadConfig;
  }

  // Called when user click in gamepad button.
  private final void onPress(final MotionEvent event) {
    mTouchPos.x = event.getX();
    mTouchPos.y = event.getY();
    mIsTouchDown = true;
    mRippleAlpha = 100;

    if (mUpRect.contains(mTouchPos.x, mTouchPos.y)) {
      mRippleClipRect = mUpRect;
    } else if (mLeftRect.contains(mTouchPos.x, mTouchPos.y)) {
      mRippleClipRect = mLeftRect;
    } else if (mDownRect.contains(mTouchPos.x, mTouchPos.y)) {
      mRippleClipRect = mDownRect;
    } else if (mRightRect.contains(mTouchPos.x, mTouchPos.y)) {
      mRippleClipRect = mRightRect;
    }

    int key = getKeyCodeEvent(mTouchPos);
    if (mGamePadListener != null && key != -1) {
      mGamePadListener.onPress(key);
      mHandler.postDelayed(mHoldRunnable, 300);
    }

    if (mRippleClipRect != null) {
      postInvalidateOnAnimation();
    }
  }

  // Called when user release gamepad button
  private final void onRelease() {
    mIsTouchDown = false;
    mHandler.removeCallbacks(mHoldRunnable);

    int key = getKeyCodeEvent(mTouchPos);
    if (mGamePadListener != null && key != -1) {
      mGamePadListener.onRelease(key);
    }

    if (mRippleClipRect != null) {
      postInvalidateOnAnimation();
    }
  }

  // Returns the keycode based on x, y.
  // used to get keycode from touch.
  private final int getKeyCodeEvent(final Vec2f pos) {
    if (mUpRect.contains(pos.x, pos.y)) {
      return KeyEvent.KEYCODE_W;
    } else if (mLeftRect.contains(pos.x, pos.y)) {
      return KeyEvent.KEYCODE_A;
    } else if (mDownRect.contains(pos.x, pos.y)) {
      return KeyEvent.KEYCODE_S;
    } else if (mRightRect.contains(pos.x, pos.y)) {
      return KeyEvent.KEYCODE_D;
    }
    return -1;
  }

  // Draw a text centered in rect
  private final void drawCenteredText(
          final Canvas canvas, final RectF rect, final String text, final Paint paint) {
    // initial sizd (too big)
    float textSize = rect.height() * 0.5f;
    paint.setTextSize(textSize);

    // decrease font size until fit in rect
    while (true) {
      final Rect bounds = new Rect();
      paint.getTextBounds(text, 0, text.length(), bounds);

      if (bounds.width() <= rect.width() * 0.7f && bounds.height() <= rect.height() * 0.5f) {
        break;
      }
      textSize -= 5f;
      paint.setTextSize(textSize);
    }

    // fix vertical alignmnt
    Paint.FontMetrics fontMetrics = paint.getFontMetrics();
    float textHeightOffset = (fontMetrics.ascent + fontMetrics.descent) / 2;

    // draw text centralized in rect
    canvas.drawText(text, rect.centerX(), rect.centerY() - textHeightOffset, paint);
  }

  @Override
  protected void onSizeChanged(final int w, final int h, final int oldw, final int oldh) {
    super.onSizeChanged(w, h, oldw, oldh);
    final float size = Math.min(w, h) / 3f;
    final float centerX = w / 2f;
    final float centerY = h / 2f;
    mUpRect.set(centerX - size / 2, centerY - size * 1.5f, centerX + size / 2, centerY - size / 2);
    mDownRect.set(
            centerX - size / 2, centerY + size / 2, centerX + size / 2, centerY + size * 1.5f);
    mLeftRect.set(
            centerX - size * 1.5f, centerY - size / 2, centerX - size / 2, centerY + size / 2);
    mRightRect.set(
            centerX + size / 2, centerY - size / 2, centerX + size * 1.5f, centerY + size / 2);
  }

  @Override
  protected void onDraw(final Canvas canvas) {
    super.onDraw(canvas);
    // Draw game pad buttons
    mPaint.setColor(Color.LTGRAY);
    canvas.drawRoundRect(mUpRect, mGamePadConfig.buttonRadius, mGamePadConfig.buttonRadius, mPaint);
    canvas.drawRoundRect(
            mLeftRect, mGamePadConfig.buttonRadius, mGamePadConfig.buttonRadius, mPaint);
    canvas.drawRoundRect(
            mDownRect, mGamePadConfig.buttonRadius, mGamePadConfig.buttonRadius, mPaint);
    canvas.drawRoundRect(
            mRightRect, mGamePadConfig.buttonRadius, mGamePadConfig.buttonRadius, mPaint);

    if (mRippleClipRect != null) {
      canvas.save();
      canvas.clipRect(mRippleClipRect);

      mRipplePaint.setAlpha(mRippleAlpha);
      final float left = mRippleClipRect.left;
      final float top = mRippleClipRect.top;
      final float right = mRippleClipRect.right;
      final float bottom = mRippleClipRect.bottom;

      canvas.drawRoundRect(left, top, right, bottom, mGamePadConfig.buttonRadius,
              mGamePadConfig.buttonRadius, mRipplePaint);
      canvas.restore();

      if (mIsTouchDown) {
        postInvalidateOnAnimation();
      } else {
        if (mRippleAlpha > 0) {
          mRippleAlpha = Math.max(0, mRippleAlpha - 6);
          postInvalidateOnAnimation();
        } else {
          // Reset everything
          mRippleAlpha = 100;
          mRippleClipRect = null;
        }
      }
    }

    // Draw game pad buttons text
    mPaint.setColor(Color.DKGRAY);
    mPaint.setTextAlign(Paint.Align.CENTER);
    drawCenteredText(canvas, mUpRect, mGamePadConfig.buttonUpText, mPaint);
    drawCenteredText(canvas, mLeftRect, mGamePadConfig.buttonLeftText, mPaint);
    drawCenteredText(canvas, mDownRect, mGamePadConfig.buttonDownText, mPaint);
    drawCenteredText(canvas, mRightRect, mGamePadConfig.buttonRightText, mPaint);
  }

  @Override
  public boolean onTouchEvent(final MotionEvent event) {
    mGestureDetector.onTouchEvent(event);
    final int action = event.getAction();
    switch (action) {
      case MotionEvent.ACTION_DOWN:
        onPress(event);
        break;
      case MotionEvent.ACTION_MOVE:
        if (mGamePadConfig.releaseOnMove) onRelease();
        break;
      case MotionEvent.ACTION_UP:
      case MotionEvent.ACTION_CANCEL:
        onRelease();
        break;
    }
    return true;
  }

  private final static class Vec2f {
    private float x;
    private float y;

    public Vec2f(float x, float y) {
      this.x = x;
      this.y = y;
    }
  }

  public final static class GamePadConfig {
    public float buttonRadius = 15;
    public String buttonUpText = "↑";
    public String buttonLeftText = "←";
    public String buttonDownText = "↓";
    public String buttonRightText = "→";
    public int buttonColor = Color.LTGRAY;
    public int buttonTextColor = Color.DKGRAY;
    public int buttonRippleColor = Color.BLACK;
    public boolean releaseOnMove = false;
    GamePadConfig() {}
  }

  public interface GamePadListener {
    void onPress(final int key);
    void onRelease(final int key);
    void onDoublePress(final int key);
    void onHold(final int key);
  }
}
