ActiveAdmin.register Resource do
  menu parent: 'Startups'

  permit_params :title, :description, :file, :thumbnail, :share_status, :shared_with_batch, :batch_id

  preserve_default_filters!

  filter :startup,
    collection: Startup.batched,
    label: 'Product',
    member_label: proc { |startup| "#{startup.product_name}#{startup.name.present? ? " (#{startup.name})" : 's'}" }

  filter :share_status,
    collection: Resource.valid_share_statuses

  index do
    selectable_column

    column :share_status do |resource|
      if resource.share_status.present?
        t("resource.share_status.#{resource.share_status}")
      end
    end

    column :shared_with_batch do |resource|
      if resource.shared_with_batch.present?
        resource.shared_with_batch
      else
        'All batches'
      end
    end

    column 'Shared with Batch (inactive)' do |resource|
      if resource.batch.present?
        link_to resource.batch.to_label, admin_batch_path(resource.batch)
      else
        'All batches'
      end
    end

    column :title
    column :downloads
    actions
  end

  show do
    attributes_table do
      row :share_status do |resource|
        if resource.share_status.present?
          t("resource.share_status.#{resource.share_status}")
        end
      end

      row :shared_with_batch do |resource|
        if resource.shared_with_batch.present?
          resource.shared_with_batch
        else
          'All batches'
        end
      end

      row 'Shared with Batch (inactive)' do |resource|
        if resource.batch.present?
          link_to resource.batch.to_label, admin_batch_path(resource.batch)
        else
          'All batches'
        end
      end

      row :title
      row :downloads
      row :description

      row :thumbnail do |resource|
        if resource.thumbnail.present?
          img src: resource.thumbnail_url
        else
          image_tag 'resources/thumbnail_default.png'
        end
      end

      row :content_type
      row :created_at
      row :updated_at
    end
  end

  form do |f|
    f.inputs 'Resource details' do
      f.input :share_status,
        as: :select,
        collection: Resource.valid_share_statuses,
        member_label: proc { |share_status| t("resource.share_status.#{share_status}") }

      f.input :shared_with_batch, placeholder: 'Leave this blank to share with all batches.'
      f.input :batch, label: 'Shared with Batch (inactive)'
      f.input :file, as: :file
      f.input :thumbnail, as: :file
      f.input :title
      f.input :description
    end

    f.actions
  end
end
