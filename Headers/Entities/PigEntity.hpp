#pragma once

#include "EntityLivingBase.hpp"
#include "Resources/ShaderProgram.hpp"
#include "Model/ArmPose.hpp"
#include "Resources/PlayerTexture.hpp"

namespace Entities
{
    class PigEntity : public EntityLivingBase
    {
    public:

        PigEntity();

        virtual void Update(float deltatime) override;
        virtual void SetupAnim(float deltaTime, float globalTime) override;
    protected:
        Model::BodyPart* head;
        Model::BodyPart* body;
        Model::BodyPart* rightHindLeg;
        Model::BodyPart* leftHindLeg;
        Model::BodyPart* rightFrontLeg;
        Model::BodyPart* leftFrontLeg;
        Resources::Texture* texture = nullptr;

        virtual Vec3 getRidePosition(EntityLivingBase* entity) override;
    };
}

/*

package net.minecraft.world.entity.animal;

import javax.annotation.Nullable;
import net.minecraft.core.BlockPos;
import net.minecraft.core.Direction;
import net.minecraft.nbt.CompoundTag;
import net.minecraft.network.syncher.EntityDataAccessor;
import net.minecraft.network.syncher.EntityDataSerializers;
import net.minecraft.network.syncher.SynchedEntityData;
import net.minecraft.server.level.ServerLevel;
import net.minecraft.sounds.SoundEvent;
import net.minecraft.sounds.SoundEvents;
import net.minecraft.sounds.SoundSource;
import net.minecraft.world.Difficulty;
import net.minecraft.world.InteractionHand;
import net.minecraft.world.InteractionResult;
import net.minecraft.world.damagesource.DamageSource;
import net.minecraft.world.entity.AgeableMob;
import net.minecraft.world.entity.Entity;
import net.minecraft.world.entity.EntityType;
import net.minecraft.world.entity.EquipmentSlot;
import net.minecraft.world.entity.ItemBasedSteering;
import net.minecraft.world.entity.ItemSteerable;
import net.minecraft.world.entity.LightningBolt;
import net.minecraft.world.entity.LivingEntity;
import net.minecraft.world.entity.Mob;
import net.minecraft.world.entity.Pose;
import net.minecraft.world.entity.Saddleable;
import net.minecraft.world.entity.ai.attributes.AttributeSupplier;
import net.minecraft.world.entity.ai.attributes.Attributes;
import net.minecraft.world.entity.ai.goal.BreedGoal;
import net.minecraft.world.entity.ai.goal.FloatGoal;
import net.minecraft.world.entity.ai.goal.FollowParentGoal;
import net.minecraft.world.entity.ai.goal.LookAtPlayerGoal;
import net.minecraft.world.entity.ai.goal.PanicGoal;
import net.minecraft.world.entity.ai.goal.RandomLookAroundGoal;
import net.minecraft.world.entity.ai.goal.TemptGoal;
import net.minecraft.world.entity.ai.goal.WaterAvoidingRandomStrollGoal;
import net.minecraft.world.entity.monster.ZombifiedPiglin;
import net.minecraft.world.entity.player.Player;
import net.minecraft.world.entity.vehicle.DismountHelper;
import net.minecraft.world.item.ItemStack;
import net.minecraft.world.item.Items;
import net.minecraft.world.item.crafting.Ingredient;
import net.minecraft.world.level.Level;
import net.minecraft.world.level.block.state.BlockState;
import net.minecraft.world.phys.AABB;
import net.minecraft.world.phys.Vec3;

public class Pig extends Animal implements ItemSteerable, Saddleable {
   private static final EntityDataAccessor<Boolean> DATA_SADDLE_ID = SynchedEntityData.defineId(Pig.class, EntityDataSerializers.BOOLEAN);
   private static final EntityDataAccessor<Integer> DATA_BOOST_TIME = SynchedEntityData.defineId(Pig.class, EntityDataSerializers.INT);
   private static final Ingredient FOOD_ITEMS = Ingredient.of(Items.CARROT, Items.POTATO, Items.BEETROOT);
   private final ItemBasedSteering steering = new ItemBasedSteering(this.entityData, DATA_BOOST_TIME, DATA_SADDLE_ID);

   public Pig(EntityType<? extends Pig> p_29462_, Level p_29463_) {
      super(p_29462_, p_29463_);
   }

   protected void registerGoals() {
      this.goalSelector.addGoal(0, new FloatGoal(this));
      this.goalSelector.addGoal(1, new PanicGoal(this, 1.25D));
      this.goalSelector.addGoal(3, new BreedGoal(this, 1.0D));
      this.goalSelector.addGoal(4, new TemptGoal(this, 1.2D, Ingredient.of(Items.CARROT_ON_A_STICK), false));
      this.goalSelector.addGoal(4, new TemptGoal(this, 1.2D, FOOD_ITEMS, false));
      this.goalSelector.addGoal(5, new FollowParentGoal(this, 1.1D));
      this.goalSelector.addGoal(6, new WaterAvoidingRandomStrollGoal(this, 1.0D));
      this.goalSelector.addGoal(7, new LookAtPlayerGoal(this, Player.class, 6.0F));
      this.goalSelector.addGoal(8, new RandomLookAroundGoal(this));
   }

   public static AttributeSupplier.Builder createAttributes() {
      return Mob.createMobAttributes().add(Attributes.MAX_HEALTH, 10.0D).add(Attributes.MOVEMENT_SPEED, 0.25D);
   }

   @Nullable
   public Entity getControllingPassenger() {
      return this.getFirstPassenger();
   }

   public boolean canBeControlledByRider() {
      Entity entity = this.getControllingPassenger();
      if (!(entity instanceof Player)) {
         return false;
      } else {
         Player player = (Player)entity;
         return player.getMainHandItem().is(Items.CARROT_ON_A_STICK) || player.getOffhandItem().is(Items.CARROT_ON_A_STICK);
      }
   }

   public void onSyncedDataUpdated(EntityDataAccessor<?> p_29480_) {
      if (DATA_BOOST_TIME.equals(p_29480_) && this.level.isClientSide) {
         this.steering.onSynced();
      }

      super.onSyncedDataUpdated(p_29480_);
   }

   protected void defineSynchedData() {
      super.defineSynchedData();
      this.entityData.define(DATA_SADDLE_ID, false);
      this.entityData.define(DATA_BOOST_TIME, 0);
   }

   public void addAdditionalSaveData(CompoundTag p_29495_) {
      super.addAdditionalSaveData(p_29495_);
      this.steering.addAdditionalSaveData(p_29495_);
   }

   public void readAdditionalSaveData(CompoundTag p_29478_) {
      super.readAdditionalSaveData(p_29478_);
      this.steering.readAdditionalSaveData(p_29478_);
   }

   protected SoundEvent getAmbientSound() {
      return SoundEvents.PIG_AMBIENT;
   }

   protected SoundEvent getHurtSound(DamageSource p_29502_) {
      return SoundEvents.PIG_HURT;
   }

   protected SoundEvent getDeathSound() {
      return SoundEvents.PIG_DEATH;
   }

   protected void playStepSound(BlockPos p_29492_, BlockState p_29493_) {
      this.playSound(SoundEvents.PIG_STEP, 0.15F, 1.0F);
   }

   public InteractionResult mobInteract(Player p_29489_, InteractionHand p_29490_) {
      boolean flag = this.isFood(p_29489_.getItemInHand(p_29490_));
      if (!flag && this.isSaddled() && !this.isVehicle() && !p_29489_.isSecondaryUseActive()) {
         if (!this.level.isClientSide) {
            p_29489_.startRiding(this);
         }

         return InteractionResult.sidedSuccess(this.level.isClientSide);
      } else {
         InteractionResult interactionresult = super.mobInteract(p_29489_, p_29490_);
         if (!interactionresult.consumesAction()) {
            ItemStack itemstack = p_29489_.getItemInHand(p_29490_);
            return itemstack.is(Items.SADDLE) ? itemstack.interactLivingEntity(p_29489_, this, p_29490_) : InteractionResult.PASS;
         } else {
            return interactionresult;
         }
      }
   }

   public boolean isSaddleable() {
      return this.isAlive() && !this.isBaby();
   }

   protected void dropEquipment() {
      super.dropEquipment();
      if (this.isSaddled()) {
         this.spawnAtLocation(Items.SADDLE);
      }

   }

   public boolean isSaddled() {
      return this.steering.hasSaddle();
   }

   public void equipSaddle(@Nullable SoundSource p_29476_) {
      this.steering.setSaddle(true);
      if (p_29476_ != null) {
         this.level.playSound((Player)null, this, SoundEvents.PIG_SADDLE, p_29476_, 0.5F, 1.0F);
      }

   }

   public Vec3 getDismountLocationForPassenger(LivingEntity p_29487_) {
      Direction direction = this.getMotionDirection();
      if (direction.getAxis() == Direction.Axis.Y) {
         return super.getDismountLocationForPassenger(p_29487_);
      } else {
         int[][] aint = DismountHelper.offsetsForDirection(direction);
         BlockPos blockpos = this.blockPosition();
         BlockPos.MutableBlockPos blockpos$mutableblockpos = new BlockPos.MutableBlockPos();

         for(Pose pose : p_29487_.getDismountPoses()) {
            AABB aabb = p_29487_.getLocalBoundsForPose(pose);

            for(int[] aint1 : aint) {
               blockpos$mutableblockpos.set(blockpos.getX() + aint1[0], blockpos.getY(), blockpos.getZ() + aint1[1]);
               double d0 = this.level.getBlockFloorHeight(blockpos$mutableblockpos);
               if (DismountHelper.isBlockFloorValid(d0)) {
                  Vec3 vec3 = Vec3.upFromBottomCenterOf(blockpos$mutableblockpos, d0);
                  if (DismountHelper.canDismountTo(this.level, p_29487_, aabb.move(vec3))) {
                     p_29487_.setPose(pose);
                     return vec3;
                  }
               }
            }
         }

         return super.getDismountLocationForPassenger(p_29487_);
      }
   }

   public void thunderHit(ServerLevel p_29473_, LightningBolt p_29474_) {
      if (p_29473_.getDifficulty() != Difficulty.PEACEFUL && net.minecraftforge.event.ForgeEventFactory.canLivingConvert(this, EntityType.ZOMBIFIED_PIGLIN, (timer) -> {})) {
         ZombifiedPiglin zombifiedpiglin = EntityType.ZOMBIFIED_PIGLIN.create(p_29473_);
         zombifiedpiglin.setItemSlot(EquipmentSlot.MAINHAND, new ItemStack(Items.GOLDEN_SWORD));
         zombifiedpiglin.moveTo(this.getX(), this.getY(), this.getZ(), this.getYRot(), this.getXRot());
         zombifiedpiglin.setNoAi(this.isNoAi());
         zombifiedpiglin.setBaby(this.isBaby());
         if (this.hasCustomName()) {
            zombifiedpiglin.setCustomName(this.getCustomName());
            zombifiedpiglin.setCustomNameVisible(this.isCustomNameVisible());
         }

         zombifiedpiglin.setPersistenceRequired();
         net.minecraftforge.event.ForgeEventFactory.onLivingConvert(this, zombifiedpiglin);
         p_29473_.addFreshEntity(zombifiedpiglin);
         this.discard();
      } else {
         super.thunderHit(p_29473_, p_29474_);
      }

   }

   public void travel(Vec3 p_29506_) {
      this.travel(this, this.steering, p_29506_);
   }

   public float getSteeringSpeed() {
      return (float)this.getAttributeValue(Attributes.MOVEMENT_SPEED) * 0.225F;
   }

   public void travelWithInput(Vec3 p_29482_) {
      super.travel(p_29482_);
   }

   public boolean boost() {
      return this.steering.boost(this.getRandom());
   }

   public Pig getBreedOffspring(ServerLevel p_149001_, AgeableMob p_149002_) {
      return EntityType.PIG.create(p_149001_);
   }

   public boolean isFood(ItemStack p_29508_) {
      return FOOD_ITEMS.test(p_29508_);
   }

   public Vec3 getLeashOffset() {
      return new Vec3(0.0D, (double)(0.6F * this.getEyeHeight()), (double)(this.getBbWidth() * 0.4F));
   }
}

package net.minecraft.client.model;

import com.google.common.collect.ImmutableList;
import net.minecraft.client.model.geom.ModelPart;
import net.minecraft.client.model.geom.PartPose;
import net.minecraft.client.model.geom.builders.CubeDeformation;
import net.minecraft.client.model.geom.builders.CubeListBuilder;
import net.minecraft.client.model.geom.builders.MeshDefinition;
import net.minecraft.client.model.geom.builders.PartDefinition;
import net.minecraft.util.Mth;
import net.minecraft.world.entity.Entity;
import net.minecraftforge.api.distmarker.Dist;
import net.minecraftforge.api.distmarker.OnlyIn;

@OnlyIn(Dist.CLIENT)
public class QuadrupedModel<T extends Entity> extends AgeableListModel<T> {
   protected final ModelPart head;
   protected final ModelPart body;
   protected final ModelPart rightHindLeg;
   protected final ModelPart leftHindLeg;
   protected final ModelPart rightFrontLeg;
   protected final ModelPart leftFrontLeg;

   protected QuadrupedModel(ModelPart p_170857_, boolean p_170858_, float p_170859_, float p_170860_, float p_170861_, float p_170862_, int p_170863_) {
      super(p_170858_, p_170859_, p_170860_, p_170861_, p_170862_, (float)p_170863_);
      this.head = p_170857_.getChild("head");
      this.body = p_170857_.getChild("body");
      this.rightHindLeg = p_170857_.getChild("right_hind_leg");
      this.leftHindLeg = p_170857_.getChild("left_hind_leg");
      this.rightFrontLeg = p_170857_.getChild("right_front_leg");
      this.leftFrontLeg = p_170857_.getChild("left_front_leg");
   }

   public static MeshDefinition createBodyMesh(int p_170865_, CubeDeformation p_170866_) {
      MeshDefinition meshdefinition = new MeshDefinition();
      PartDefinition partdefinition = meshdefinition.getRoot();
      partdefinition.addOrReplaceChild("head", CubeListBuilder.create().texOffs(0, 0).addBox(-4.0F, -4.0F, -8.0F, 8.0F, 8.0F, 8.0F, p_170866_), PartPose.offset(0.0F, (float)(18 - p_170865_), -6.0F));
      partdefinition.addOrReplaceChild("body", CubeListBuilder.create().texOffs(28, 8).addBox(-5.0F, -10.0F, -7.0F, 10.0F, 16.0F, 8.0F, p_170866_), PartPose.offsetAndRotation(0.0F, (float)(17 - p_170865_), 2.0F, ((float)Math.PI / 2F), 0.0F, 0.0F));
      CubeListBuilder cubelistbuilder = CubeListBuilder.create().texOffs(0, 16).addBox(-2.0F, 0.0F, -2.0F, 4.0F, (float)p_170865_, 4.0F, p_170866_);
      partdefinition.addOrReplaceChild("right_hind_leg", cubelistbuilder, PartPose.offset(-3.0F, (float)(24 - p_170865_), 7.0F));
      partdefinition.addOrReplaceChild("left_hind_leg", cubelistbuilder, PartPose.offset(3.0F, (float)(24 - p_170865_), 7.0F));
      partdefinition.addOrReplaceChild("right_front_leg", cubelistbuilder, PartPose.offset(-3.0F, (float)(24 - p_170865_), -5.0F));
      partdefinition.addOrReplaceChild("left_front_leg", cubelistbuilder, PartPose.offset(3.0F, (float)(24 - p_170865_), -5.0F));
      return meshdefinition;
   }

   protected Iterable<ModelPart> headParts() {
      return ImmutableList.of(this.head);
   }

   protected Iterable<ModelPart> bodyParts() {
      return ImmutableList.of(this.body, this.rightHindLeg, this.leftHindLeg, this.rightFrontLeg, this.leftFrontLeg);
   }

   public void setupAnim(T p_103509_, float p_103510_, float p_103511_, float p_103512_, float p_103513_, float p_103514_) {
      this.head.xRot = p_103514_ * ((float)Math.PI / 180F);
      this.head.yRot = p_103513_ * ((float)Math.PI / 180F);
      this.rightHindLeg.xRot = Mth.cos(p_103510_ * 0.6662F) * 1.4F * p_103511_;
      this.leftHindLeg.xRot = Mth.cos(p_103510_ * 0.6662F + (float)Math.PI) * 1.4F * p_103511_;
      this.rightFrontLeg.xRot = Mth.cos(p_103510_ * 0.6662F + (float)Math.PI) * 1.4F * p_103511_;
      this.leftFrontLeg.xRot = Mth.cos(p_103510_ * 0.6662F) * 1.4F * p_103511_;
   }
}

package net.minecraft.client.model;

import net.minecraft.client.model.geom.ModelPart;
import net.minecraft.client.model.geom.PartPose;
import net.minecraft.client.model.geom.builders.CubeDeformation;
import net.minecraft.client.model.geom.builders.CubeListBuilder;
import net.minecraft.client.model.geom.builders.LayerDefinition;
import net.minecraft.client.model.geom.builders.MeshDefinition;
import net.minecraft.client.model.geom.builders.PartDefinition;
import net.minecraft.world.entity.Entity;
import net.minecraftforge.api.distmarker.Dist;
import net.minecraftforge.api.distmarker.OnlyIn;

@OnlyIn(Dist.CLIENT)
public class PigModel<T extends Entity> extends QuadrupedModel<T> {
   public PigModel(ModelPart p_170799_) {
      super(p_170799_, false, 4.0F, 4.0F, 2.0F, 2.0F, 24);
   }

   public static LayerDefinition createBodyLayer(CubeDeformation p_170801_) {
      MeshDefinition meshdefinition = QuadrupedModel.createBodyMesh(6, p_170801_);
      PartDefinition partdefinition = meshdefinition.getRoot();
      partdefinition.addOrReplaceChild("head", CubeListBuilder.create().texOffs(0, 0).addBox(-4.0F, -4.0F, -8.0F, 8.0F, 8.0F, 8.0F, p_170801_).texOffs(16, 16).addBox(-2.0F, 0.0F, -9.0F, 4.0F, 3.0F, 1.0F, p_170801_), PartPose.offset(0.0F, 12.0F, -6.0F));
      return LayerDefinition.create(meshdefinition, 64, 32);
   }
}

*/