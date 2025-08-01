package dev.trindadedev.nativeassets;

import android.content.Context;
import android.content.res.AssetManager;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.ByteArrayOutputStream;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.List;

public class AndroidAssetsManager {
  private final AssetManager assetManager;
  private final String assetsRoot;

  public AndroidAssetsManager(final Context ctx, final String root) {
    this.assetManager = ctx.getAssets();
    this.assetsRoot = root;
  }

  public String readTextFile(final String path) {
    try {
      InputStream inputStream = assetManager.open(assetsRoot + path);
      BufferedReader reader = new BufferedReader(new InputStreamReader(inputStream));
      StringBuilder content = new StringBuilder();
      String line;
      while ((line = reader.readLine()) != null) {
        content.append(line).append('\n');
      }
      return content.toString();
    } catch (IOException e) {
      e.printStackTrace();
    }
    return "";
  }

  public byte[] readBinaryFile(final String path) {
    try (InputStream inputStream = assetManager.open(assetsRoot + path);
            ByteArrayOutputStream buffer = new ByteArrayOutputStream()) {
      byte[] tmp = new byte[4096];
      int bytesRead;
      while ((bytesRead = inputStream.read(tmp)) != -1) {
        buffer.write(tmp, 0, bytesRead);
      }

      return buffer.toByteArray();

    } catch (IOException e) {
      e.printStackTrace();
      return new byte[0];
    }
  }

  public boolean fileExists(final String path) {
    try {
      final InputStream is = assetManager.open(assetsRoot + path);
      is.close();
      return true;
    } catch (IOException e) {
      e.printStackTrace();
    }
    return false;
  }

  public String[] listFiles(final String path) {
    try {
      String[] files = assetManager.list(assetsRoot + path);
      if (files != null) {
        return files;
      }
    } catch (IOException e) {
      e.printStackTrace();
    }
    return new String[0];
  }
}